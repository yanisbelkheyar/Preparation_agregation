#include <stdlib.h>


//fonction de fusion de deux tableau trie en un tableau trie contenant les éléments des deux tableau précédent 
void fusion_tableau(int* tab_res, int* tab, int debut_gauche, int debut_droit, int taille_gauche, int taille_droit,int indice){
   
    if(taille_gauche==0 && taille_droit==0)
        return;

    if(taille_gauche==0){
        for(int i=0;i<taille_droit;i++)
            tab_res[indice+i] = tab[debut_droit+i];
    }else if (taille_droit==0){
        for(int i=0;i<taille_gauche;i++)
            tab_res[indice+i] = tab[debut_gauche+i];
    }else{
        if(tab[debut_gauche]<tab[debut_droit]){
            tab_res[indice] = tab[debut_gauche];
            fusion_tableau(tab_res,tab,debut_gauche+1,debut_droit,taille_gauche-1,taille_droit,indice+1);
        }else{
            tab_res[indice] = tab[debut_droit];
            fusion_tableau(tab_res,tab,debut_gauche,debut_droit+1,taille_gauche,taille_droit-1,indice+1);
        }
    }
}


//fonction auxiliaire pour l'appelle recursif de la fonction de trie fusion
void trie_fusion_rec(int* tab, int debut, int fin){

    if(debut+1==fin)
        return;

    int taille = fin-debut;
    int debut_gauche = debut;
    int fin_gauche = debut + ((taille)/2) ;
    int debut_droit = debut + ((taille)/2) ;
    int fin_droit = fin;

    if(fin_gauche >= debut_gauche)
        trie_fusion_rec(tab,debut_gauche,fin_gauche);
    if(fin_droit >= debut_droit)
        trie_fusion_rec(tab,debut_droit,fin_droit);
    
    int* tab_tmp = malloc(sizeof(int)*taille);
    for(int i=0;i<taille;i++){
        tab_tmp[i] = tab[debut+i];
    }

    fusion_tableau(tab,tab_tmp,0,fin_gauche-debut_gauche,fin_gauche-debut_gauche,fin_droit-debut_droit,debut);

    free(tab_tmp); 
}


void trie_fusion(int* tab,int taille){

    trie_fusion_rec(tab,0,taille);

}