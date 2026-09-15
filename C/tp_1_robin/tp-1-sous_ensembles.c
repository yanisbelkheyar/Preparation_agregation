#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

void print_all_subsets(uint8_t present[], int i, int n) {
    if (i < n) {
        present[i] = 0;
        print_all_subsets(present, i + 1, n);
        present[i] = 1;
        print_all_subsets(present, i + 1, n);
    } else {
        printf("{");
        bool first = true;
        for (unsigned j = 0; j < n; ++j) {
            if (!present[j]) { continue; }
            if (first) {
                printf("%d", j + 1);
                first = false;
            } else {
                printf(", %d", j + 1);
            }
        }
        printf("}\n");
    }
}

int main(int _argc, char *_argv[]) {
    printf("Ce programme va afficher tous les sous-ensembles non vides de\nl'ensemble {1, ..., n} pour un certain entier n > 0.\n\n");

    int n;
    bool valid_n;
    do {
        printf("Merci de saisir la valeur de n :");
        scanf("%d", &n);
        valid_n = n > 0;
        if (!valid_n) {
            printf("Vous devez saisir un entier strictement positif !\n\n");
        }
    } while (!valid_n);

    printf("Voici tous les sous-ensembles non vides de l'ensemble {");
    for (int i = 1; i <= n; ++i) {
        printf(i > 1 ? ", %d" : "%d", i);
    }
    printf("} :\n");

    uint8_t *present = (uint8_t *) malloc(n * sizeof(uint8_t));
    
    print_all_subsets(present, 0, n);

    free(present);
    return 0;
}