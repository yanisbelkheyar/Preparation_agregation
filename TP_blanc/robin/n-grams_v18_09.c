#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Choix faits:
 * Je ne gère que les chaines ASCII, plutôt que du texte unicode arbitraire.
    Cela correspond à l'exemple du sujet ("ca va" plutôt que "ça va")
    , évite de devoir gérer les différentes façon dont un même caractère peut-être représenté en unicode (e.g. 'é' vs '\'' suivi de 'e')
    , et permet d'utiliser de simples tableaux de taille 128.
 * construire_un_gramme prend son texte en entrée comme un simple char[] terminé par le caractère null.
    Il aurait aussi été possible de prendre la taille du texte comme argument, j'ai choisi d'être idiomatique plutôt.
*/

/* Question 1 */

enum { ASCII_TAILLE = 128 };

typedef struct {
    char char_le_plus_vu;
    unsigned occurence_max;
    unsigned occurences_total;
    unsigned occurences[ASCII_TAILLE];
} successeurs_t;

typedef struct {
    successeurs_t *succs[ASCII_TAILLE];
} un_gramme_t;

const successeurs_t successeurs_vide = {
    ' ',
    0,
    0,
    {0}
};

const successeurs_t *get_successeurs(const un_gramme_t *modele, char c) {
    if (modele->succs[c] == NULL) {
        return &successeurs_vide;
    }
    return modele->succs[c];
}

bool est_char_ascii(char c) {
    return c >= 0; // pas besoin de comparer à ASCII_TAILLE, puisqu'un char signé ne peut jamais dépasser 127.
}

unsigned nombre_occurences(un_gramme_t *modele, char dernier_char, char char_suivant) {
    assert(est_char_ascii(dernier_char));
    assert(est_char_ascii(char_suivant));

    return get_successeurs(modele, dernier_char)->occurences[char_suivant];
}

unsigned nombre_occurences_total(un_gramme_t *modele, char c) {
    assert(est_char_ascii(c));

    return get_successeurs(modele, c)->occurences_total;
}

typedef struct {
    unsigned n1;
    unsigned n2;
} pair_unsigned_t;

pair_unsigned_t proba_conditionnelle_ratio(un_gramme_t *modele, char dernier_char, char char_suivant) {
    assert(est_char_ascii(dernier_char));
    assert(est_char_ascii(char_suivant));
    pair_unsigned_t resultat = {
        nombre_occurences(modele, dernier_char, char_suivant)
        , nombre_occurences_total(modele, dernier_char)
    };
    return resultat;
}

// Renvoie 0.0 si dernier_char n'a jamais été vu
float proba_conditionnelle_float(un_gramme_t *modele, char dernier_char, char char_suivant) {
    assert(est_char_ascii(dernier_char));
    assert(est_char_ascii(char_suivant));
    pair_unsigned_t ratio = proba_conditionnelle_ratio(modele, dernier_char, char_suivant);
    if (ratio.n2 == 0) {
        return 0.0;
    }
    return ((float) ratio.n1) / ((float) ratio.n2);
}

/* Question 2 */

void liberer_tous_successeurs(un_gramme_t *modele) {
    for (size_t i = 0; i < ASCII_TAILLE; ++i) {
        if (modele->succs[i] != NULL) {
            free(modele->succs[i]);
        }
    }
}

void ajouter_occurence(successeurs_t *succs, char c) {
    assert(est_char_ascii(c));
    succs->occurences_total++;
    unsigned new_n = ++succs->occurences[c];
    if (new_n > succs->occurence_max) {
        succs->occurence_max = new_n;
        succs->char_le_plus_vu = c;
    }
}

successeurs_t * construire_successeurs() {
    successeurs_t *resultat = (successeurs_t *) malloc(sizeof(successeurs_t));
    resultat->occurence_max = 0;
    resultat->occurences_total = 0;
    for (size_t i = 0; i < ASCII_TAILLE; ++i) {
        resultat->occurences[i] = 0;
    }
    return resultat;
}

void construire_un_gramme(un_gramme_t *modele, char texte[]) {
    for (size_t i = 0; i < ASCII_TAILLE; ++i) {
        modele->succs[i] = NULL;
    }

    char dernier_char = texte[0];
    if (dernier_char == '\0') {
        return;
    }

    for (size_t i = 1; texte[i] != '\0'; ++i) {
        /* Invariants:
         * le modele est correct pour texte[0, i-1]
         * dernier_char == texte[i-1]
         */
        char char_suivant = texte[i];
        assert(est_char_ascii(char_suivant));
        if (NULL == modele->succs[dernier_char]) {
            modele->succs[dernier_char] = construire_successeurs();
        }
        ajouter_occurence(modele->succs[dernier_char], char_suivant);
        dernier_char = char_suivant;
    }
}

/* Question 3 */

char prediction_char (const un_gramme_t *modele, char c) {
    const successeurs_t *succs = get_successeurs(modele, c);
    char resultat = succs->char_le_plus_vu;
    assert(succs->occurences[resultat] == succs->occurence_max);
    return resultat;
}

/* prediction doit être de taille au moins n + 1.
 * sequence_entree doit être une chaine de caractères ascii valide, terminée par un '\0', et avec au moins un autre caractère.
 * Ecrit les n caractères prédits dans prediction, suivi de '\0'.
 * Ne modifie ni modele ni sequence_entree.
 */
void predire_sequence(const un_gramme_t *modele, const char sequence_entree[], char prediction[], size_t n) {
    assert(sequence_entree[0] != '\0');
    char dernier_char = sequence_entree[0];
    for (size_t i = 1; sequence_entree[i] != '\0'; ++i) {
        dernier_char = sequence_entree[i];
    }

    for (size_t i = 0; i < n; ++i) {
        prediction[i] = prediction_char(modele, dernier_char);
        dernier_char = prediction[i];
    }
    prediction[n] = '\0';
}

/* Tests */

void assert_ratio(pair_unsigned_t p1, unsigned n1, unsigned n2) {
    assert(p1.n1 == n1);
    assert(p1.n2 == n2);
}

void test_construire_un_gramme() {
    un_gramme_t modele;
    construire_un_gramme(&modele, "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux.");
    assert_ratio(proba_conditionnelle_ratio(&modele, 'a', 'l'), 2, 6);
    assert_ratio(proba_conditionnelle_ratio(&modele, 'a', ' '), 3, 6);
    assert_ratio(proba_conditionnelle_ratio(&modele, 'a', ','), 1, 6);
    assert_ratio(proba_conditionnelle_ratio(&modele, 'n', 'j'), 1, 3);
    assert_ratio(proba_conditionnelle_ratio(&modele, 'n', 't'), 1, 3);
    assert_ratio(proba_conditionnelle_ratio(&modele, 'n', ' '), 1, 3);
    liberer_tous_successeurs(&modele);
}

void test_prediction() {
    un_gramme_t modele;
    construire_un_gramme(&modele, "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux.");
    size_t n = 10;
    char resultat[n + 1];
    predire_sequence(&modele, "Bonjour, ", resultat, n);
    assert(strcmp(resultat, "cour, cour") == 0); // Vérifié à la main
    liberer_tous_successeurs(&modele);
}

int main(int _argc, char *_argv[]) {
    test_construire_un_gramme();
    test_prediction();
}