#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "objet.h"

typedef struct donnees donnees;
typedef struct objet objet;

void remplir_tab(int **tab, objet *tab_objets, int n, int pmax){
    for(int i=0; i<n; i++){
        for(int j=0; j<=pmax; j++){
            if(i==0){
                if(j>=tab_objets[i].p){
                    tab[i][j] = tab_objets[i].v;
                }
                else {
                    tab[i][j] = 0;
                }
            }
            else {
                if(j<tab_objets[i].p){
                    tab[i][j] = tab[i-1][j];
                }
                else {
                    int val1 = tab[i-1][j];
                    int val2 = tab_objets[i].v+tab[i-1][j-tab_objets[i].p];
                    tab[i][j] = (val1>=val2?val1:val2);
                }
            }
        }
    }
    return;
}

int main(int argc, char **argv){
    assert(argc==2);
    char *filename = argv[1];
    donnees *mes_donnees = creer_tab_objets(filename);
    objet *tab_objets = mes_donnees->tab;
    int n = mes_donnees->n;
    int pmax = mes_donnees->pmax;
    int **tab = malloc(sizeof(int*)*n);
    for(int i=0; i<n; i++){
        tab[i] = malloc(sizeof(int)*(pmax+1));
    }
    remplir_tab(tab, tab_objets, n, pmax);
    int valeur_opti = tab[n-1][pmax];
    printf("La valeur optimale est %i !\n", valeur_opti);
    /*Il faut encore tout free*/
    free(tab_objets);
    free(mes_donnees);
    for(int i=0; i<n; i++){
        free(tab[i]);
    }
    free(tab);
    return 0;
}