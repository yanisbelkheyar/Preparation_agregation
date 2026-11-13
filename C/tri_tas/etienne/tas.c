#include "tas.h"

tas creer(int l) {
    int* tab=malloc(l*sizeof(int));
    tas new_tas={tab,l,0};
    return (new_tas);
};

int get_min(tas* t) {
    if (est_vide(t)) {
        return INT_MIN;
    }
    else {
        return (t->tab[0]);
    }
};

bool est_plein(tas* t){
    return(t->length==t->count);
};

bool est_vide(tas* t){
    return(t->count==0);
};

void print(tas* t) {
    printf("Length: %d\n", t->length);
    printf("Count: %d\n", t->count);
    for (int i=0;i<t->count;i++){
        printf("%d ",t->tab[i]);
    }
    printf("\n");
    return;
};

int echange_up(tas* t,int i){
    if (i==0) return -1;
    int pere=floor((i-1)/2);
    if (t->tab[pere]<=t->tab[i]) {
        return -1;
    }
    else {
        int tmp=t->tab[pere];
        t->tab[pere]=t->tab[i];
        t->tab[i]=tmp;
        return pere;
    }
}

bool inserer(tas* t,int val){
    if (est_plein(t)){
        return false;
    }
    t->tab[t->count]=val;
    int i=t->count;
    t->count++;
    while (i!=-1){
        i=echange_up(t,i);
    }
    return(true);
};

int echange_down(tas* t,int i){
    int gauche=2*i+1;
    int droit=2*i+2;
    if (gauche>t->count) return -1; //feuille
    int ind_min_fils;
    if (droit>t->count) ind_min_fils=gauche;//gauche est la seule feuille
    else {
        if (t->tab[gauche]<=t->tab[droit]){
            ind_min_fils=gauche;
        }
        else {
            ind_min_fils=droit;
        }
    }
    if (t->tab[i]<=t->tab[ind_min_fils]) {return -1;}
    else {
        int tmp=t->tab[ind_min_fils];
        t->tab[ind_min_fils]=t->tab[i];
        t->tab[i]=tmp;
        return ind_min_fils;
    }
}

int supprime_min(tas* t){
    int min_val=get_min(t);
    if (min_val==INT_MIN) return INT_MIN;
    t->tab[0]=t->tab[t->count-1];
    t->count--;
    int i=0;
    while (i!=-1){
        i=echange_down(t,i);
    }
    return (min_val);
}

void tris_tas(int* tab, int l){
    tas t=creer(l);
    for (int i=0;i<l;i++){
        inserer(&t,tab[i]);
    }
    // print(&t);
    for (int i=0;i<l;i++){
        tab[i]=supprime_min(&t);
    }
}