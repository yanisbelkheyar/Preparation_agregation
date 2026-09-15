#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

bool check_valid_length(int n) {
    if (n > 0) {
        return true;
    }
    printf("Vous devez saisir un entier strictement positif !\n\n");
    return false;
}

void print_array(int t[], int n) {
    printf("Le tableau de %d entiers saisi est le suivant :\n[", n);
    for (unsigned i = 0; i < n; ++i) {
        printf(i > 0 ? ", %d" : "%d", t[i]);
    }
    printf("]\n");
}

bool check_sorted(int t[], int n) {
    for (unsigned i = 1; i < n; ++i) {
        if (t[i - 1] > t[i]) {
            return false;
        }
    }
    return true;
}

// Returns -1 if the key is not found in t[low..high), and its index otherwise.
// Requires t to be sorted in increasing order.
int search_aux(int t[], int k, size_t low, size_t high) {
    if (low >= high) {
        return -1;
    }

    // Properties true of the recursive call:
    // - k is in t[low..high) iff it is in t[new_low..new_high)
    // - (new_high - new_low) <= (high - low) / 2
    size_t mid = low + (high - low) / 2;
    if (t[mid] == k) {
        return mid;
    } else if (t[mid] < k) {
        return search_aux(t, k, mid+1, high);
    } else {
        assert(t[mid] > k);
        return search_aux(t, k, low, mid);
    }
}

int search(int t[], int k, int n) {
    return search_aux(t, k, 0, (size_t) n);
}

int main(int _argc, char *_argv[]) {
    printf("Ce programme va verifier si, dans un tableau d'entiers tries par ordre croissant,\n\
que vous allez saisir, se trouve un element (entier) donne,\n\
que vous allez egalement saisir.\n\n");

    int n;
    do {
        printf("Saisir d'abord le nombre d'elements du tableau trie : ");
        scanf("%d", &n);
    } while (!check_valid_length(n));

    int *t = (int *) malloc(n * sizeof(int));

    for (unsigned i = 0; i < n; ++i) {
        printf("Saisir l'element no %d (sur %d) de ce tableau trie d'entiers : ", i + 1, n);
        scanf("%d", &t[i]);
    }

    print_array(t, n);

    if (!check_sorted(t, n)) {
        free(t);
        printf("Les elements de ce tableau ne sont pas tries par ordre croissant !\n");
        return 1;
    } else {
        printf("Les elements de ce tableau sont bien tries par ordre croissant !\n\n");
    }

    int k;
    printf("Saisir maintenant l'element recherche dans ce tableau : ");
    scanf("%d", &k);

    int idx = search(t, k, n);
    if (idx == -1) {
        printf("L'entier %d n'a pas ete trouve !\n", k);
    } else {
        printf("L'entier %d se trouve en case %d du tableau !\n", k, idx + 1);
    }

    free(t);
    return 0;
}