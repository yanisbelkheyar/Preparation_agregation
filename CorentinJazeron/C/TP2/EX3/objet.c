#include <stdio.h>
#include <stdlib.h>

#include "objet.h"

void lire_fichier(FILE *f, Obj **tab_obj, int *nb_obj, int *p_max) {
	int ch;

	fscanf(f, "%d", nb_obj);
	while ((ch = fgetc(f)) != '\n' && ch != EOF) {};

	fscanf(f, "%d", p_max);
	while ((ch = fgetc(f)) != '\n' && ch != EOF) {};

	*tab_obj = malloc((*nb_obj)*sizeof(Obj));
	for (int i = 0; i < *nb_obj; ++i) {
		fscanf(f, "%d,%d", &((*tab_obj)[i].p), &((*tab_obj)[i].v));
		while ((ch = fgetc(f)) != '\n' && ch != EOF) {};
	}
}