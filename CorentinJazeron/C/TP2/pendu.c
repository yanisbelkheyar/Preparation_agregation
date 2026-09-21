#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

const int nb_max_erreur = 6;
int nb_erreur = 0;

char *choisir_mot(FILE *f) {
	int n;
	fscanf(f, "%d", &n);
	int c; while((c = fgetc(f)) != '\n' && c != EOF) continue;

	int n_choice = rand()%n;
	char temp[100];
	for (int i = 0; i < n_choice; ++i) {
		fscanf(f, "%s", &temp);
		int c; while((c = fgetc(f)) != '\n' && c != EOF) continue;
	}

	int len = strlen(temp);
	char *choice = malloc((len+1)*sizeof(char));
	for (int i = 0; i < len; ++i) {
		choice[i] = temp[i];
	} choice[len] = '\0';
	return choice;
}

void tour_de_jeu(char *mot, int size, bool visited[256]) {
	printf("Mot : ");
	for (int i = 0; i < size; ++i) {
		if (visited[(unsigned char) mot[i]]) {
			printf("%c", mot[i]);
		} else {
			printf("_");
		}
	} printf("\n");

	printf("Lettre saisies : ");
	for (int c = 0; c < 256; ++c) {
		if (visited[(unsigned char) c]) {
			printf("%c ", c);
		}
	} printf("\n");

	printf("Nombre d'essais restants : %d\n", nb_max_erreur - nb_erreur);

	char c;
	bool flag = false;
	do {
		printf("Entrez une lettre : ");
		c = getchar();
		int ch; while((ch = getchar()) != '\n' && ch != EOF) continue;

		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '-'))) {
			printf("Vous devez entrer une lettre de l'alphabet !\n");
		} else if (visited[(unsigned char) c]) {
			printf("Vous devez saisir une lettre que vous n'avez pas encore saisie !\n");
		} else {
			flag = true;
		}
	} while (!flag);

	bool ind = false;
	for (int i = 0; i < size; ++i) {
		if (mot[i] == c) {
			ind = true;
		}
	}

	if (ind) {
		printf("Bravo, le mot contient la lettre %c !\n", c);
	} else {
		printf("Désolé, le mot ne contient pas la lettre %c !\n", c);
		++nb_erreur;
	}
	visited[(unsigned char) c] = true;

	printf("\n");
}

int fin(char *mot, int size, bool visited[256]) {
	if (nb_erreur >= nb_max_erreur) {
		return 2;
	}
	for (int i = 0; i < size; ++i) {
		if (!visited[(unsigned char) mot[i]]) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		printf("Syntaxe attendue : pendu [nom_du_fichier].\n");
		return 0;
	}
	srand(clock());

	FILE *f = fopen(argv[1], "r");
	char *mot = choisir_mot(f);
	int size = strlen(mot);

	printf("Vous avez %d erreurs, à vous de jouer !\n", nb_max_erreur);
	bool visited[256] = {0};

	int flag = 0;
	while (flag == 0) {
		tour_de_jeu(mot, size, visited);
		flag = fin(mot, size, visited);
	}

	if (flag == 1) {
		printf("Bravo, vous avez deviné le mot %s en %d erreurs !\n", mot, nb_erreur);
	} else if (flag == 2) {
		printf("Vous n'avez pas réussi à deviner le mot %s en moins de %d erreurs !\n", mot, nb_erreur);
	}

	free(mot);
	fclose(f);
	return 0;
}