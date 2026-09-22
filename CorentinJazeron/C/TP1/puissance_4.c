#include <stdio.h>
#include <stdlib.h>

enum etat_type {CONT, DRAW, W_J1, W_J2};
typedef enum etat_type etat;

enum joueur_type {J1, J2};
typedef enum joueur_type joueur;

enum tcase_type {FREE, C1, C2};
typedef enum tcase_type tcase;

const char char_j1 = 'X';
const char char_j2 = 'O';
const char char_blank = '_';

int nb_tokens = 0;
tcase grille[6][7] = {{FREE}};

void affiche_grille(tcase g[6][7]) {
	printf("1234567\n\n");
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 7; ++j) {
			switch (g[i][j]) {
				case FREE:
					printf("%c", char_blank);
				break;
				case C1:
					printf("%c", char_j1);
				break;
				case C2:
					printf("%c", char_j2);
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}

etat gagnant(tcase c) {
	return (c == C1)?W_J1:W_J2;
}

etat test_win(tcase g[6][7]) {
	tcase c;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 7; ++j) {
			c = g[i][j];
			if (c == FREE) continue;

			/* horizontale */
			if (j + 3 < 7
			    && g[i][j+1] == c && g[i][j+2] == c && g[i][j+3] == c)
				return gagnant(c);

			/* verticale */
			if (i + 3 < 6
			    && g[i+1][j] == c && g[i+2][j] == c && g[i+3][j] == c)
				return gagnant(c);

			/* diagonale bas-droite */
			if (i + 3 < 6 && j + 3 < 7
			    && g[i+1][j+1] == c && g[i+2][j+2] == c && g[i+3][j+3] == c)
				return gagnant(c);

			/* diagonale bas-gauche */
			if (i + 3 < 6 && j - 3 >= 0
			    && g[i+1][j-1] == c && g[i+2][j-2] == c && g[i+3][j-3] == c)
				return gagnant(c);
		}
	}

	if (nb_tokens >= 6 * 7) return DRAW;
	return CONT;
}

/* Renvoie la ligne où tomberait un jeton dans cette colonne,
   ou -1 si la colonne est invalide ou pleine. */
int premiere_ligne(int colonne, tcase g[6][7]) {
	if (colonne < 0 || colonne >= 7) return -1;
	for (int i = 0; i < 6; ++i) {
		if (g[i][colonne] != FREE) return i - 1;
	}
	return 6 - 1;
}

void lire_colonne(int *colonne) {
	int flag;
	
	flag = scanf("%d", colonne);
	if (flag != 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF) continue;
	}

	--(*colonne);
}

void jouer(tcase g[6][7], joueur j, const char *nom) {
	printf("%s (%c), entrez la colonne où vous souhaitez jouer : ",
	       nom, (j == J1)?char_j1:char_j2);

	int colonne, ligne, flag;

	lire_colonne(&colonne);
	ligne = premiere_ligne(colonne, g);

	while (ligne < 0 && flag != 1) {
		printf("Colonne invalide, veuillez recommencer : ");

		flag = scanf("%d", &colonne);
		if (flag != 1) {
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF) continue;
		}

		--colonne;
		ligne = premiere_ligne(colonne, g);
	}

	g[ligne][colonne] = (j == J1)?C1:C2;
	++nb_tokens;
}

int main(int argc, char const *argv[]) {
	const char *j1 = argv[1];
	const char *j2 = argv[2];

	printf("Bienvenue pour une nouvelle partie de Puissance 4 entre %s et %s !\n", j1, j2);
	affiche_grille(grille);

	etat s = CONT;
	joueur j = J1;
	while (s == CONT) {
		jouer(grille, j, (j == J1)?j1:j2);
		printf("\n");
		affiche_grille(grille);
		s = test_win(grille);
		j = (j == J1) ? J2 : J1;
	}

	switch (s) {
		case DRAW: printf("Match nul !\n"); break;
		case W_J1: printf("%s a gagné !\n", j1); break;
		case W_J2: printf("%s a gagné !\n", j2); break;
	}

	return 0;
}