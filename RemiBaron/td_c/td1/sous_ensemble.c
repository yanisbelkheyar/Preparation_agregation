#include <stdio.h>
#include <stdlib.h>

void print_tab(int *t, int n){
    printf("{");
    int num_elt = 0;
    for (int i=0; i<n; i++){
        if(t[i]==1){
            if(num_elt!=0){
                printf(", ");
            }
            else {
                num_elt++;
            }
            printf("%i",i+1);
        }
    }
    printf("}\n");
}

void rec_fun(int *t, int n, int i){
    if (i==n){
        print_tab(t,n);
        return;
    }
    t[i]=0;
    rec_fun(t,n,i+1);
    t[i]=1;
    rec_fun(t,n,i+1);
}

int main(int argc, char **argv){
    int n = 0;
    do {
        printf("Bonjour, bonjour ! Saisissez un nombre strictement positif svp ! ");
        scanf("%d",&n);
    } while(n<=0);
    printf("Merci, merci. Voici la liste de tous les sous-ensembles possibles de [|1,%i|]\n",n);
    int *t = malloc(n*sizeof(int));
    rec_fun(t,n,0);
    free(t);
}