#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "objet.h"

int max(int a, int b) {
    return a > b ? a : b;
}

int main(int argc, char const *argv[]) {
	assert(argc == 2);

	FILE *f = fopen(argv[1], "r");

	Obj *tab_obj;
	int nb_obj;
	int p_max;
	lire_fichier(f, &tab_obj, &nb_obj, &p_max);
	fclose(f);

	int **t = malloc(nb_obj*sizeof(int*));
	for (int i = 0; i < nb_obj; ++i) {
		t[i] = malloc((p_max+1)*sizeof(int*));
	}

	for (int j = 0; j <= p_max; ++j) {
		t[0][j] = (j >= tab_obj[0].p ? tab_obj[0].v : 0);
	}

	for (int i = 1; i < nb_obj; ++i) {
		for (int j = 0; j <= p_max; ++j) {
			if (j >= tab_obj[i].p) {
				t[i][j] = max(tab_obj[i].v + t[i][j-tab_obj[i].p], t[i-1][j]);
			} else {
				t[i][j] = t[i-1][j];
			}
		}
	}

	printf("La valeur optimale est %d !\n", t[nb_obj-1][p_max]);

	for (int i = 0; i < nb_obj; ++i) {
		free(t[i]);
	} free(t);
	return 0;
}