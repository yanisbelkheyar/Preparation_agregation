#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Vous avez saisi %d pays, que voici:\n", argc - 1);
    for (int i = 1; i < argc; i++) {
        printf("\t%s\n", argv[i]);
    }
    return 0;
}