#include <stdio.h>
#include "Trie_rapide.h"

void print_tableau(int *tableau,int taille_tab){
    printf("\n");
    for(int i=0;i<taille_tab;i++){
        printf("%d ",tableau[i]);
    }
    printf("\n");
}

int pivot_last(int* tab,int debut,int fin){
    return fin;
}

void trie_rapide_rec(int* tab, int debut, int fin, int (*trouver_pivot)(int*,int,int)){
    if (fin==debut || debut > fin){
        return;
    }

    int pivot = trouver_pivot(tab,debut,fin);
    //printf("pivot = %d at pos %d\n",tab[pivot],pivot);
    //printf("debut = %d, fin = %d\n",debut,fin);
    int ppg = debut;
    for(int i=debut;i<fin;i++){

        if(tab[i]<=tab[pivot]){
            int tmp = tab[i];
            tab[i] = tab[ppg];
            tab[ppg] = tmp;
            ppg +=1;
        }
    }

    int tmp = tab[pivot];
    tab[pivot] = tab[ppg];
    tab[ppg] = tmp;

    //print_tableau(tab,10);

    trie_rapide_rec(tab, debut, ppg-1,trouver_pivot);
    trie_rapide_rec(tab, ppg+1,fin,trouver_pivot);
}

//effectue le trie rapide en place du tableau tab
void trie_rapide(int* tab,int taille_tab){
    trie_rapide_rec(tab,0,taille_tab-1,pivot_last);
}