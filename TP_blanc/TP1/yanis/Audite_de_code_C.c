#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

//Correction : ajout stdlio pour affichage
#include <stdio.h>

typedef struct Cell { int value; struct Cell *next; } list;

// J’ai trouvé un algorithme vraiment merveilleux ! R. F.
bool list_cyclic(list *l) {

  //Correction : Nom de variable en anglais
  //Correction : Pas de vérification que la liste est vide et
  //donc que le l-> next n'est pas impossible
  list *tortoise = l, *hare = l->next;
  
  while (tortoise != hare) {
    //Correction : Non utilisation des valeur true et false pour les retour 
    if (hare == NULL) return 0;
    //Correction : Probleme dans le cas ou l'élement hare->next == NULL
    hare = hare->next->next;
    tortoise = tortoise->next;
  }

  //Correction : Non utilisation des valeur true et false pour les retour 
  return 1;
}

//Description du programme précédant:
//Double parcour de liste simultané:
//un premier ou l'on parcour les élément un par un avec la variable tortue
//l'autre ou l'on les parcour 2 par 2 avec le lapin
//Le nom de la fonction laisse a penser que le but du programme était
//de faire une detection de cycle dans une liste.
//Si la liste est circulaire alors le liévre devrais ratraper la tortue,
//si ce n'est pas le cas alors le lievre va atteindre le bout de la liste en premier.

bool list_cyclic_corriger(list* l){

  if(l == NULL || l->next == NULL){
    return false;
  }
  
  list *tortue = l, *lievre = l->next;
  
  while (tortue != lievre) {
    if (lievre == NULL) return false;
    if (lievre->next != NULL){
      if (lievre->next->next != NULL){
	      lievre = lievre->next->next;
      }else{
	      return false;
      }
    }else{
      return false;
    }
    tortue = tortue->next;
  }
  return true;
}


// Et j’ai bien vérifié qu’il fonctionne correctement.
int main() {

  list l1, l2, l3;
    l1.next = &l2;
    l2.next = &l3;
    l3.next = NULL;
    assert(!list_cyclic(&l1));
    l3.next = &l1;
    assert(list_cyclic(&l1));
    assert(list_cyclic(&l2));
    assert(list_cyclic(&l3));
 
  //Correction : manque de test des cas critique de type liste vide,
  //taille de liste non multiple de 2k+1
  //Correction : Proposition de nouveau jeux de test

  //Correction : test list vide
  assert(!list_cyclic_corriger(NULL));

  //Correction : test list cilcique a un élément
  l1.next = &l1;
  assert(list_cyclic_corriger(&l1));

  //Correction : test pour des taille de 2 a 100 de liste, cyclique et non cyclique
  int taille_liste_test = 100;

  list l1_new;
  list suiv;

  l1_new.next = &suiv;

  list tableau_noeud[taille_liste_test];
  //Correction : test pour des listes non cyclique
  for(int i=0;i<taille_liste_test-1;i++){
    list l_cur;
    tableau_noeud[i] = l_cur;
    suiv.next = &tableau_noeud[i];
    suiv = tableau_noeud[i];
    assert(!list_cyclic_corriger(&l1_new));
  }
  printf(" Test list non cyclique ok\n");

  //Correction : test pour des listes cyclique
 
  for(int i=0;i<taille_liste_test-1;i++){
    list l_cur;
    suiv.next = &l1_new;
    assert(list_cyclic_corriger(&l1_new));
    suiv.next = &l_cur;
    suiv = l_cur;
  }
  printf(" Test list cyclique ok\n");
  
}
