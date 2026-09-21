#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Cell { int value; struct Cell *next; } list;

// J’ai trouvé un algorithme vraiment merveilleux ! R. F.
bool old_list_cyclic(list *l) {
    list *tortoise = l, *hare = l->next;
    while (tortoise != hare) {
        if (hare == NULL) return 0;
        hare = hare->next->next;
        tortoise = tortoise->next;
    }
    return 1;
}

// Et j’ai bien vérifié qu’il fonctionne correctement.
int old_main() {
    list l1, l2, l3;
    l1.next = &l2;
    l2.next = &l3;
    l3.next = NULL;
    assert(!old_list_cyclic(&l1));
    l3.next = &l2;
    assert(old_list_cyclic(&l1));
    assert(old_list_cyclic(&l2));
    assert(old_list_cyclic(&l3));
}

// Problème 1 (correction): rate le cas où hare->next est NULL (et où hare->next->next segfaults)
// Problème 2 (correction): rate le cas où l est NULL (et où hare = l->next segfaults)
// Problème 3 (tests): manque de cas pour les tests: listes purement cyclique, des cas où la section pré-cycle/dans le cycle sont de tailles paires/impaires, liste vide.

// Problème 4 (commentaires): les commentaires sont inutiles, et il manque d'un commentaire expliquant pourquoi l'algorithme est correct (incluant la terminaison)
// Problème 5 (warning): type de main + manque un return dans main (mineur/discutable)
// Problème 6 (style): return 0/1 pour des booléens (mineur / discutable)

bool list_cyclic(list *l) {
    if (l == NULL) {
        return false;
    }
    list *tortoise = l, *hare = l->next;
    while (tortoise != hare) {
        if (hare == NULL || hare->next == NULL) {
            return false;
        }
        hare = hare->next->next;
        tortoise = tortoise->next;
    }
    return true;
}

enum { max_taille_liste_pour_tests = 10 };
void assert_sur_liste_cyclique(int num_avant_cycle, int num_apres_cycle) {
    assert(num_avant_cycle >= 0);
    assert(num_apres_cycle >= 0);
    int num_noeuds = num_avant_cycle + num_apres_cycle;
    assert(num_noeuds > 0);
    assert(num_noeuds < max_taille_liste_pour_tests);
    list tous_noeuds[max_taille_liste_pour_tests];
    for (size_t i = 0; i < num_noeuds - 1; ++i) {
        tous_noeuds[i].value = 0;
        tous_noeuds[i].next = &tous_noeuds[i + 1];
    }
    tous_noeuds[num_noeuds - 1].value = 0;
    tous_noeuds[num_noeuds - 1].next = &tous_noeuds[num_avant_cycle];
    assert(list_cyclic(&tous_noeuds[0]));
}

void assert_sur_liste_non_cyclique(int n) {
    assert(n > 0);
    assert(n < max_taille_liste_pour_tests);
    list tous_noeuds[max_taille_liste_pour_tests];
    for (size_t i = 0; i < n - 1; ++i) {
        tous_noeuds[i].value = 0;
        tous_noeuds[i].next = &tous_noeuds[i + 1];
    }
    tous_noeuds[n - 1].value = 0;
    tous_noeuds[n - 1].next = NULL;
    assert(!list_cyclic(&tous_noeuds[0]));
}

int main (int _argc, char *_argv[]) {
    assert(!list_cyclic(NULL));
    for (int i = 1; i < 4; ++i) {
        assert_sur_liste_non_cyclique(i);
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i + j > 0) {
                assert_sur_liste_cyclique(i, j);
            }
        }
    }
    return 0;
}