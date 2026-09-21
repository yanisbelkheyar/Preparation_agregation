typedef struct objet {
	int p;
	int v;
} Obj;

void lire_fichier(FILE *f, Obj **tab_obj, int *nb_obj, int *p_max);