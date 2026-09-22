#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sous_ensemble {
	int n;
	int *t;
} Sous_ensemble ;

void affiche_set(Sous_ensemble s) {
	bool fst = true;

	printf("{");
	for (int i = 0; i < s.n; ++i) {
		if (s.t[i]) {
			if (fst) {
				printf("%d", i+1);
				fst = false;
			} else {
				printf(", %d", i+1);
			}
		}
	}
	printf("}\n");
}

void calcule_set(Sous_ensemble s, int i) {
	if (i < s.n) {
		s.t[i] = 1;
		calcule_set(s, i+1);
		s.t[i] = 0;
		calcule_set(s, i+1);
	} else {
		affiche_set(s);
	}
}

int main(int argc, char const *argv[]) {
	int n, flag;

	printf("Veuillez entrer un entier strictement positif :\n");
	do {
		flag = scanf("%d", &n);
		if (flag != 1) {
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF) continue;
		}
	} while(flag != 1 && n <= 0);

	Sous_ensemble s;
	s.n = n;
	s.t = malloc(n * sizeof(char));

	calcule_set(s, 0);

	free(s.t);
	return 0;
}