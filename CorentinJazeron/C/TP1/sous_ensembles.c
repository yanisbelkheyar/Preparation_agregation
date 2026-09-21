#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void affiche_set(char *t, int n) {
	bool fst = true;

	printf("{");
	for (int i = 0; i < n; ++i) {
		if (t[i]) {
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

void calcule_set(char *t, int n, int i) {
	if (i < n) {
		t[i] = 1;
		calcule_set(t, n, i+1);
		t[i] = 0;
		calcule_set(t, n, i+1);
	} else {
		affiche_set(t, n);
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

	char *t = malloc(n * sizeof(char));

	calcule_set(t, n, 0);

	free(t);
	return 0;
}