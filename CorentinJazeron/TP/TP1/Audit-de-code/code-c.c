#include <stdbool.h> // pourquoi ?
#include <stdlib.h> // pourquoi bis ?
#include <assert.h>

// mieux defini comme une cellule
typedef struct Cell { int value; struct Cell *next; } cell;

// J’ai trouvé un algorithme vraiment merveilleux ! R. F.
bool list_cyclic(list *l) {
	if (l == NULL) return false; // Problème d'accès si le liste est vide
	list *tortoise = l, *hare = l->next;
	while (tortoise != hare) {
		// On verifie aussi que hare n'est pas le dernier element de la liste
		if (hare == NULL || hare->next == NULL) return false;
		hare = hare->next->next;
		tortoise = tortoise->next;
	}
	return true;
}

// Et j’ai bien vérifié qu’il fonctionne correctement.
int main() {
	// declaration plus propre
	cell *l1, *l2, *l3;
	l1->next = l2;
	l2->next = l3;
	l3->next = NULL;
	assert(!list_cyclic(l1));
	assert(!list_cyclic(l2)); // tests complets
	assert(!list_cyclic(l3));
	l3->next = l2;
	assert(list_cyclic(l1));
	assert(list_cyclic(l2));
	assert(list_cyclic(l3));

	assert(!list_cyclic(NULL)); // test sur la liste vide

	return 0; // return de la main
}