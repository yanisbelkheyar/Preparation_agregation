#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_trie(int *t, int n){
    for (int i=1; i<n; i++){
        if (t[i]<t[i-1]){
            return false;
        }
    }
    return true;
}

void dichotomie (int *t, int n, int elt){
    int a = 0;
    int b = n;
    while(b-a>1){
        int mid = (a+b)/2;
        if(elt>=t[mid]){
            a = mid;
        }
        else {
            b=mid;
        }
    }
    if (t[a]==elt){
        printf("\n\nL'entier %i se trouve en case %i du tableau !\n", elt, a);
    }
    else {
        printf("\n\nL'entier %i n'a pas ete trouve !\n", elt);
    }
}

int main(int argc, int **argv){
    printf("Ce programme va verifier si, dans un tableau d'entiers tries par ordre croissant, que vous allez saisir, se trouve un element (entier) donne, que vous allez egalement saisir.\n\n");

    int n;
    do{
        printf("Saisir d'abord le nombre d'elements du tableau trie : ");
        scanf("%d",&n);
        if(n<=0){
            printf("Vous devez saisir un entier strictement positif !\n");
        }
    } while(n<=0);

    int *t = malloc(n*sizeof(int));
    int elt;
    for(int i = 0; i<n;i++){
        printf("Saisir l'element no %i (sur %i) de ce tableau trie d'entiers : ", i+1, n); 
        scanf("%d",&elt);
        t[i] = elt;
    }
    printf("\nLe tableau de %i entiers saisi est le suivant :\n[",n);
    for(int i=0; i<n-1;i++){
        printf("%i ",t[i]);
    }
    printf("%i]\n",t[n-1]);

    if(is_trie(t,n)){
        printf("Le tableau est bien trié par ordre croissant!\n\n");
        printf("Saisir maintenant l'element recherche dans ce tableau : ");
        int elt_recherche;
        scanf("%d",&elt_recherche);
        dichotomie(t, n, elt_recherche);
    }
    else {
        printf("\nTableau non trié golem !\n");
    }
    free(t);
}