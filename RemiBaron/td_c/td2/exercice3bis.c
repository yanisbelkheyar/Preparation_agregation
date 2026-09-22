#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "objet.h"

/*version O(Pmax) en espace*/

typedef struct donnees donnees;
typedef struct objet objet;

void remplir_tab(int *tab, objet *tab_objets, int n, int pmax){
    /*Remplir pour i = 0 (i=1 dans l'énoncé)*/
    for(int j=0; j<=pmax; j++){
        if(j>= tab_objets[0].p){
            tab[j] = tab_objets[0].v;
        }
        else {
            tab[j] = 0;
        }
    }
    /*Remplir pour i>=1 en commençant par la dernière colonne à chaque fois*/
    for(int i=1; i<n; i++){
        for(int j=pmax; j>=0; j--){
            /*Si j<pi alors tab[j] ne change pas, sinon on fait le même calcul que dans l'ancienne version sachant que les valeurs de tab sont avec i-1*/
                if(j>=tab_objets[i].p){
                    int val1 = tab[j];
                    int val2 = tab_objets[i].v+tab[j-tab_objets[i].p];
                    tab[j] = (val1>=val2?val1:val2);
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
    int *tab = malloc(sizeof(int)*(pmax+1));
    remplir_tab(tab, tab_objets, n, pmax);
    int valeur_opti = tab[pmax];
    printf("La valeur optimale est %i !\n", valeur_opti);
    /*Il faut encore tout free*/
    free(tab_objets);
    free(mes_donnees);
    free(tab);
    return 0;
}