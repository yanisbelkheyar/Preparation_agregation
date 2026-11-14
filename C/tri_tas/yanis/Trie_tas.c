#include "Trie_tas.h"

//Creation d'un tas a partire d'un tableau d'entier non necessairement trié
//Renvoi le tas contenant les même élément que le tableau
tas* creation_tas_depuis_tableau(int tab[],int taille_tab){
    if(taille_tab==0){
        return NULL;
    }

    tas* t = cree_tas(taille_tab);
    for(int i=0;i<taille_tab;i++){
        if(!ajout_tas(t,tab[i]))
            return NULL;
    }

    return t;

}

void print_tableau(int *tableau,int taille_tab){
    printf("\n");
    for(int i=0;i<taille_tab;i++){
        printf("%d ",tableau[i]);
    }
    printf("\n");
}



//On extrait sucessivement depuis le tas t le min du tas et on l'ajoute sucessivement au tableau initial
void extraction_sucessive_min_tas(tas *t,int *tab,int taille_tab){
    if(t->taille_max==0)
        return;

    for(int i=0;i<taille_tab;i++){
        //print_tas(t);
        //print_tableau(tab,i);
        int tmp = min_tas(t);
        //printf("%d min tas\n",tmp);
        tab[i] = tmp;
        if(!supression_tas(t))
            return;
    }

}

