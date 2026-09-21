#include <stdio.h>
#include <stdlib.h>

typedef struct tableau {
    int *tab;
    int taille;
} tableau;

void print_ss_ens(tableau *t){
    printf("{");
    int premier_element = 0;
    for(int i=0; i<t->taille; i++){
        if((t->tab)[i] ==1){
            if(premier_element){
                printf(", ");
            }
            else {
                premier_element++;
            }
            printf("%d",i+1);
        }
    }
    printf("}\n");
}

void ss_tableaux(tableau *t, int i){
    if(t->taille == i){
        print_ss_ens(t);
        return;
    }
    (t->tab)[i] = 0;
    ss_tableaux(t, i+1);
    (t->tab)[i] = 1;
    ss_tableaux(t, i+1);
}

tableau *creer_tableau(int n){
    tableau *t = malloc(sizeof(tableau));
    int *tab = malloc(sizeof(int)*n);
    t->tab = tab;
    for(int i=0; i<n; i++){
        tab[i] = i;
    }
    t->taille = n;
    return t;
}

int main(int argc, char **argv){
    int n;
    do{
        printf("\nMerci de saisir la valeur de n :");
        scanf("%d", &n);
    }while(n<=0);
    tableau *t = creer_tableau(n);
    printf("\nVoici les sous-ensembles du tableau [0,...,%d]:\n",n);
    ss_tableaux(t, 0);
}
