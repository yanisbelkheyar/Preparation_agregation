#include <stdio.h>

int main(int nb_args, char *tab_args[]){
    printf("Vous avez %i pays, que voici :\n", nb_args-1);
    for(int i = 1; i<nb_args; i++){
        printf("%s\n",tab_args[i]);
    }
    return 0;
}