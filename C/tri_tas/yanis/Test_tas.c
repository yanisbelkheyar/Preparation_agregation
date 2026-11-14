#include "Tas.h"
#include <assert.h>

//Fichier pour le test de l'implémentation des tas

//test de la creation de tas
bool test_cree_tas(){
    tas *t1 = cree_tas(10);
    assert(t1!=NULL);
    assert(t1->taille_max==10);
    tas *t2 = cree_tas(0);
    assert(t2->taille_max==0);
    return true;
}


//test de l'accession a la valeur min du tas
bool test_min_tas(){

    tas* t1 = cree_tas(10);
    t1->tab[0] = 2;
    t1->nbr_elt = 1;
    assert(min_tas(t1)==2);
    return true;
}


//test de la capacité courant du tas
bool test_plein_tas(){
    tas* t1 = cree_tas(10);
    t1->nbr_elt = 10;
    assert(est_plein(t1)==true);
    t1->nbr_elt = 4;
    assert(est_plein(t1)==false);
    return true;
}


//test du vide sur le tas
bool test_vide_tas(){
    tas* t1 = cree_tas(10);
    t1->nbr_elt = 10;
    assert(est_vide(t1)==false);
    t1->nbr_elt = 0;
    assert(est_vide(t1)==true);    
    return true;
}

//test ajout de valeur
bool test_ajout_tas(){

    tas *t = cree_tas(10);
    print_tas(t);

    ajout_tas(t,10);

    assert(t->nbr_elt==1);
    assert(est_plein(t)==false);
    assert(min_tas(t)==10);
    print_tas(t);

    ajout_tas(t,2);

    assert(t->nbr_elt==2);
    assert(est_plein(t)==false);
    printf("valeur min = %d\n %d",min_tas(t),min_tas(t)==2);
    //assert(min_tas(t)==2);
    print_tas(t);

    ajout_tas(t,5);
    ajout_tas(t,15);
    ajout_tas(t,1);

    assert(t->nbr_elt==5);
    assert(est_plein(t)==false);
    printf("valeur min = %d\n %d",min_tas(t),min_tas(t)==2);
    assert(min_tas(t)==1);
    print_tas(t);
}

//test supression de valeur
bool test_supression_tas(){

    printf("supression test\n");

    tas *t = cree_tas(10);
    ajout_tas(t,2);
    ajout_tas(t,10);
    ajout_tas(t,5);
    ajout_tas(t,15);
    ajout_tas(t,1);

    assert(t->nbr_elt==5);
    assert(est_plein(t)==false);
    assert(min_tas(t)==1);
    print_tas(t);

    supression_tas(t);
    print_tas(t);

    assert(t->nbr_elt==4);
    assert(est_plein(t)==false);
    assert(min_tas(t)==2);

    return true;
}


int main(){

        test_cree_tas();
        test_min_tas();
        test_plein_tas();
        test_vide_tas();

        test_ajout_tas();
        test_supression_tas();
    return 1;
}