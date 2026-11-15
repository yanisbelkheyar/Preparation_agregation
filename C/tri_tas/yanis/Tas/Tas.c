#include "Tas.h"

//affiche le contenue d'un tas min sous la forme de son tableau de valeur
void print_tas(tas *t){
    printf("\nContenue du tas \n");
    for(int i=0;i<t->nbr_elt;i++){
        printf("%d,",t->tab[i]);
    }
    printf("\n");
}

//methode pour la manipulation des tas

//creation de tas vide
// Renvoi un pointeur vers un tas vide
tas* cree_tas(int taille_max){
    tas *t = malloc(sizeof(tas));
    t->nbr_elt = 0;
    t->taille_max = taille_max;
    t->profondeur = 0;
    t->tab = malloc(sizeof(int)*taille_max);

    return t;
}

//accesseur de tas
//Renvoi le plus petit élément stocker dans un tas
int min_tas(tas *t){
    if(t->nbr_elt==0){
        printf("\nerreur, tas vide\n");
        exit;
    }
    return t->tab[0];
}

//verification 
//Renvoi true si le nombre d'élément du tas est égale a la taille maximum du tas
bool est_plein(tas *t){
    if(t->taille_max == t->nbr_elt)
        return true;
    else
        return false;
}

//verification
//Renvoi true si le nombre d'élément du tas est egale a 0
bool est_vide(tas *t){
    if(t->nbr_elt == 0)
        return true;
    else
        return false;
}


//fonction auxiliére pour échanger des valeur dans un tas min afin d'obtenir un tas min a la suite d'un ajout de valeur
void reordonne_valeur_tas_bas_vers_haut(tas *t,int indice){
    if(indice == 0){
        return;
    }
    if(t->tab[indice] < t->tab[(indice-1)/2]){
        int tmp = t->tab[indice];
        t->tab[indice] = t->tab[(indice-1)/2];
        t->tab[(indice-1)/2] = tmp;
        reordonne_valeur_tas_bas_vers_haut(t,(indice-1)/2);
    }
}

//ajout d'élément dans le tas
//vérifie si le tas n'est pas plein, et si ce n'est pas le cas ajoute la valeur 
//a la position adéquoite dans le tas.
//renvoi true si la valeur a pu être ajouter et false sinon
bool ajout_tas(tas *t, int valeur){
    if(est_plein(t))
        return false;

    t->tab[t->nbr_elt] = valeur;
    reordonne_valeur_tas_bas_vers_haut(t,t->nbr_elt);
    t->nbr_elt += 1;

    if(t->nbr_elt/2 != (t->nbr_elt-1)/2)
        t->profondeur += 1;

    return true;
}


//fonction auxiliére pour échanger des valeur dans un tas min afin d'obtenir un tas min a la suite d'un ajout de valeur
void reordonne_valeur_tas_haut_vers_bas(tas *t,int indice){
    if((indice*2 + 1) >= t->nbr_elt){
        return;
    }

    //printf("valeur courante = %d \n",t->tab[indice]);
    //print_tas(t);
    //printf("enfant = %d %d (%d)\n",t->tab[indice*2+1],t->tab[indice*2+2],t->tab[indice] >= t->tab[indice*2+1] && t->tab[indice] >= t->tab[indice*2+2]);

    if(t->tab[indice] >= t->tab[indice*2+1] || t->tab[indice] >= t->tab[indice*2+2]){
        
        if(t->tab[indice*2+1]<=t->tab[indice*2+2]){
            int tmp = t->tab[indice];
            t->tab[indice] = t->tab[indice*2+1];
            t->tab[indice*2+1] = tmp;
            reordonne_valeur_tas_haut_vers_bas(t,indice*2+1);
        }else{
            int tmp = t->tab[indice];
            t->tab[indice] = t->tab[indice*2+2];
            t->tab[indice*2+2] = tmp;
            reordonne_valeur_tas_haut_vers_bas(t,indice*2+2);
        }
    }
}

//supression de lá racine d'un tas min
//Verfifier si le tas n'est pas vide,
//si ce n'est pas le cas on extrait la racine, et on la remplace par le dernier élément du tableau
//Puis on va faire des échange successif entre le noeud et ses enfant tant que le noeud est plus grande que au moins un de ces enfants.
//renvoi true si la valeur a put être suprimer et false sinon
bool supression_tas(tas *t){
    if(est_vide(t))
        return false;

    if(t->nbr_elt==1){
        t->nbr_elt = 0;
        return true;
    }

    t->tab[0] = t->tab[t->nbr_elt-1];
    t->nbr_elt -= 1;

    if(t->nbr_elt/2 != (t->nbr_elt+1)/2)
        t->profondeur -= 1;

    reordonne_valeur_tas_haut_vers_bas(t,0);
    return true;
}


