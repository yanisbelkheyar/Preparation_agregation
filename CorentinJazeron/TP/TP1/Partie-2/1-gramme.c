#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/* Successeurs d'un 1-gramme donne : qui suit ce caractere, et combien de fois */
typedef struct {
	int occ[256]; // occ[s] = nb de fois ou le caractere s suit ce 1-gramme
	int total; // somme des occ[s] : sert de denominateur pour P(s|c)
	bool present; // distingue un 1-gramme vu sans successeur d'un 1-gramme absent
} Successeurs;

/* Modele complet : une entree par caractere possible, indexee par son code */
typedef struct {
	Successeurs table[256]; // table[c] decrit les successeurs du caractere c
	int nb_grammes; // nombre de 1-grammes distincts presents
} Modele1G;


/* Renvoie P(s|c), la probabilite de s parmi les successeurs de c, et
	vaut 0.0 si c est absent du modele ou sans successeur */
double probabilite(Modele1G *m, unsigned char c, unsigned char s) {
	Successeurs *suc = &m->table[c];
	if (suc->total == 0) return 0.0;
	return (double) suc->occ[s] / (double) suc->total;
}


/* Prolonge seq d'au plus max caracteres, en choisissant a
	chaque etape le successeur le plus probable selon le modele m
   Renvoie une chaine de caracteres allouee, a liberer */
char *predire_texte(Modele1G *m, char *seq, int max) {
	if (m == NULL || seq == NULL || max < 0) return NULL;

	int n = strlen(seq);
	if (n == 0) return NULL;

	char *pred = malloc((n + max + 1) * sizeof(char));
	if (pred == NULL) return NULL;

	for (int i = 0; i < n; ++i) {
		pred[i] = seq[i];
	} pred[n] = '\0';

	for (int i = 0; i < max; ++i) {
		int count = 0;
		int occ_next_char = 0;
		Successeurs *suc = &m->table[(unsigned char) pred[n-1+i]];

		// count est le nb de 1-grammes avec l'occurence maximale
		for (int c = 0; c < 256; ++c) {
			if (suc->occ[c] > occ_next_char) {
				count = 1;
				occ_next_char = suc->occ[c];
			} else if (suc->occ[c] == occ_next_char) {
				++count;
			}
		}

		if (occ_next_char == 0) break;  // sans successeur

		// on en choisit un au hasard
		int choice = rand()%count;
		int next_char = -1;
		int j = 0;
		for (int c = 0; c < 256; ++c) {
			if (suc->occ[c] == occ_next_char) {
				if (j == choice) {
					next_char = c;
				}
				++j;
			}
		}
		assert(next_char >= 0);

		pred[n+i] = (char) next_char;
		pred[n+i+1] = '\0'; // pour toujours avoir un mot bien forme
	}

	return pred;
}


/* Construit le modele associe au texte texte
   Renvoie un modele alloue a liberer par liberer_modele */
Modele1G *construire_modele(char *texte) {
	if (texte == NULL) return NULL;

	Modele1G *m = calloc(1, sizeof(Modele1G)); // tout initialise a 0
	if (m == NULL) return NULL;

	int n = strlen(texte);
	for (int i = 0; i < n; ++i) {
		// cast obligatoire : un char signe donnerait un indice negatif au-dela de 127
		unsigned char c = (unsigned char) texte[i];

		if (!m->table[c].present) {
			m->table[c].present = true;
			++(m->nb_grammes);
		}

		if (i + 1 < n) {  // le dernier caractere du texte n'a pas de successeur
			unsigned char s = (unsigned char) texte[i+1];
			++(m->table[c].occ[s]);
			++(m->table[c].total);
		}
	}

	return m;
}


/* Libere le modele m */
void liberer_modele(Modele1G *m) {
	free(m);
}

/* Teste la fonction predire_texte */
void test_predire_text() {
	char *texte = "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux.";
	Modele1G *m = construire_modele(texte);
	assert(m != NULL);

	char *p;

	// tests sur des cas 'normaux'
	p = predire_texte(m, "B", 20);
	assert(p != NULL);
	printf("\"B\" + 20 -> \"%s\"\n", p);
	free(p);

	p = predire_texte(m, "le", 6);
	assert(p != NULL);
	printf("\"le\" + 6 -> \"%s\"\n", p);
	free(p);

	// test prediction de 0 caracteres
	p = predire_texte(m, "Ca", 0);
	assert(p != NULL);
	printf("\"Ca\" + 0 -> \"%s\"\n", p);
	free(p);

	// la prediction s'arrête avant les 4 caractères demandés
	p = predire_texte(m, "x", 4);
	assert(p != NULL);
	printf("\"x\" + 4 -> \"%s\"\n", p);
	free(p);

	// '.' est present mais sans successeur
	p = predire_texte(m, "mieux.", 30);
	assert(p != NULL);
	printf("\"mieux.\" + 30 -> \"%s\"\n", p);
	free(p);

	// 'Z' absent du modele
	p = predire_texte(m, "Z", 8);
	assert(p != NULL);
	printf("\"Z\" + 8 -> \"%s\"\n", p);
	free(p);

	assert(predire_texte(NULL, "Ca", 3) == NULL);
	assert(predire_texte(m, NULL, 3) == NULL);
	assert(predire_texte(m, "Ca", -1) == NULL);
	assert(predire_texte(m, "", 3) == NULL);

	liberer_modele(m);
}


/* Test pour les octets >= 128 */
void test_octets_hauts() {
	Modele1G *m = construire_modele("\xC3\xA9\xC3\xA9");
	assert(m != NULL && m->table[0xC3].occ[0xA9] == 2);

	char *p = predire_texte(m, "\xC3", 3);
	assert(p != NULL && strcmp(p, "\xC3\xA9\xC3\xA9") == 0);
	free(p);

	liberer_modele(m);
}


int main(void) {
	srand(time(NULL));

	test_predire_text();
	test_octets_hauts();
	
	return 0;
}