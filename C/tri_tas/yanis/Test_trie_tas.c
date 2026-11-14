#include "Trie_tas.h"
#include <assert.h>

//Fonction de test pour le trie par tas
bool est_trier(int *tab,int taille_tab){
    for(int i=0;i<taille_tab-1;i++){
        if(tab[i] > tab[i+1])
            return false;
    }
    return true;
}

//genere un tableau d'entier aléatoire
void generer_tableau(int taille_tab, int *tab){
    for(int i=0;i<taille_tab;i++){
        tab[i] = rand()%taille_tab;
    }
}


int main(){

    int nbr_test = 100;
    int taille_tableau = 100;
    for(int i=0;i<nbr_test;i++){
        int *tab = malloc(sizeof(int)*taille_tableau);
        generer_tableau(taille_tableau,tab);

        tas* t = creation_tas_depuis_tableau(tab,taille_tableau);
        print_tas(t);
        extraction_sucessive_min_tas(t,tab,taille_tableau);
        print_tableau(tab,taille_tableau);
        
        assert(est_trier(tab,taille_tableau)==true);
    }

    return 1;
}