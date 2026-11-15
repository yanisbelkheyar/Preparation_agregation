#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//structure pour l'implementation de tas
typedef struct{
    int profondeur;
    int nbr_elt;
    int taille_max;
    int *tab;
}tas;

void print_tas(tas *t);

//methode pour la manipulation des tas

//creation de tas vide
// Renvoi un pointeur vers un tas vide
tas* cree_tas(int taille_max);

//accesseur de tas
//Renvoi le plus petit élément stocker dans un tas
int min_tas(tas *t);

//verification 
//Renvoi true si le nombre d'élément du tas est égale a la taille maximum du tas
bool est_plein(tas *t);

//verification
//Renvoi true si le nombre d'élément du tas est egale a 0
bool est_vide(tas *t);

//ajout d'élément dans le tas
//vérifie si le tas n'est pas plein, et si ce n'est pas le cas ajoute la valeur 
//a la position adéquoite dans le tas.
//renvoi true si la valeur a pu être ajouter et false sinon
bool ajout_tas(tas *t, int valeur);


//supression de lá racine d'un tas min
//Verfifier si le tas n'est pas vide,
//si ce n'est pas le cas on extrait la racine, et on la remplace par le dernier élément du tableau
//Puis on va faire des échange successif entre le noeud et ses enfant tant que le noeud est plus grande que au moins un de ces enfants.
//renvoi true si la valeur a put être suprimer et false sinon
bool supression_tas(tas *t);