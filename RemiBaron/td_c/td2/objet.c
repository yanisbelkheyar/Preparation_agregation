#include <stdio.h>
#include <stdlib.h>
#include "objet.h"

typedef struct donnees donnees;
typedef struct objet objet;

donnees *creer_tab_objets(char *filename){
    donnees *rep = malloc(sizeof(donnees));
    int n;
    int pmax;
    FILE *f = fopen(filename, "r");
    fscanf(f, "%d", &n);
    fscanf(f, "%d", &pmax);
    objet *tab = malloc(sizeof(objet)*n);
    for(int i=0; i<n; i++){
        int vi, pi;
        fscanf(f, "%d,%d", &pi, &vi);
        tab[i].p = pi;
        tab[i].v = vi;
    }
    fclose(f);
    rep->n = n;
    rep->pmax = pmax;
    rep->tab = tab;
    return rep;
}