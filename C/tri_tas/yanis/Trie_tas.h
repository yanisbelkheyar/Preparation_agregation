#include "Tas.h"

//Creation d'un tas a partire d'un tableau d'entier non necessairement trié
//Renvoi le tas contenant les même élément que le tableau
tas* creation_tas_depuis_tableau(int tab[],int taille_tab);


//On extrait sucessivement depuis le tas t le min du tas et on l'ajoute sucessivement au tableau initial
void extraction_sucessive_min_tas(tas *t,int *tab,int taille_tab);

//affichage du tableau trié
void print_tableau(int *tableau,int taille_tab);