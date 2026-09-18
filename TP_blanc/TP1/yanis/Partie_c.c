/*Dans ce fichier nous traiterons de la partie 1-grammes du TP*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "Partie_c.h"

int nombre_1_grammes = 256;

// ---- constructeur de N_grammes -----

//Prend en entrée le caractére du 1-gramme, 
// la liste des caractére qui lui succedent et le nombre d'occurence du 1-gramme
//Initialise le 1-gramme, calcule le caratére le plus probable a sucesséde au 1-gramme
//Renvoi le 1-gramme crée
N_grammes construit_n_grammes(char caractere,
			      char* sucesseur,
			      int nombre_occurence_1_gramme){
    
    int* liste_sucesseur = calloc(nombre_1_grammes,sizeof(int));

    int maximum_occurence = 0;
    int indice_maximum_occurence = 0;

    for(int i=0;i<strlen(sucesseur);i++){
        liste_sucesseur[sucesseur[i]]++;

        if(liste_sucesseur[sucesseur[i]]>maximum_occurence){
            indice_maximum_occurence = sucesseur[i];
            maximum_occurence = liste_sucesseur[sucesseur[i]];
        }
    }

    Suivant sucesseur_grammes = {.sucesseur = indice_maximum_occurence, 
        .occurence = maximum_occurence};

    N_grammes nouveau_n_grammes = {.caractere = caractere, 
        .successeur_grammes = sucesseur_grammes, 
        .occurence = nombre_occurence_1_gramme};

    free(liste_sucesseur); 
    return nouveau_n_grammes;
}

//Contrat de pré et poscondition de la fonction de construction de 1-grammes 
N_grammes contrat_construit_n_grammes(char caractere,
				      char* sucesseur,
				      int nombre_occurence_1_gramme){

    assert(strlen(sucesseur) <= nombre_occurence_1_gramme &&
	   strlen(sucesseur) > nombre_occurence_1_gramme - 1);
    N_grammes resultat = construit_n_grammes(
					     caractere,
					     sucesseur,
					     nombre_occurence_1_gramme);
    assert(resultat.caractere == caractere);
    assert(resultat.occurence == nombre_occurence_1_gramme);

}

/* ---- Test -----*/

//Test l'initialisation d'un 1-gramme avec l'example donnée dans l'énoncé
void test_a_gramme_example(){
    char* texte = "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux.";

    char* a_gramme_sucesseur = "   ,ll";
    int taille = 6;
    char a_gramme = 'a';

    N_grammes A_grammes = construit_n_grammes(a_gramme,a_gramme_sucesseur,taille);

        printf("%c gramme, sucesseur %c avec proba %d / %d \n",A_grammes.caractere, 
        A_grammes.successeur_grammes.sucesseur,
        A_grammes.successeur_grammes.occurence, A_grammes.occurence);


    assert (A_grammes.successeur_grammes.sucesseur==' ');
    assert (A_grammes.successeur_grammes.occurence==3);

}


/* --- initialisation du model ---*/

//Prend en entrée un text et sa taille
//Crée un model de 1-gramme en initialisant chaque 1-gramme
//Renvoi le model de 1-gramme crée
model_N_grammes initialise_N_grammes(char* text,
				     int taille_text){

    char tableau_grames[nombre_1_grammes][taille_text];
    int taille_sucesseur_1_grammes[nombre_1_grammes];
    bool initialiser[nombre_1_grammes];

    //Récupération de chacun des 1-grammes et de la liste des caractéres
    //possible sucedant a chaque 1-gramme
    for(int i=0;i<taille_text;i++){
        if(!initialiser[text[i]]){
            initialiser[text[i]] = true;
        }
        if(i<taille_text-1){
            tableau_grames[text[i]][taille_sucesseur_1_grammes[text[i]]] = text[i+1];
            taille_sucesseur_1_grammes[text[i]]++;
        }
    }

    //Initialisation de chaque 1-gramme avec le constructeur précédant.

    model_N_grammes model;
    for(int i=0;i<nombre_1_grammes;i++){
        if(initialiser[i]){
            model.liste_un_gramme[i] =
	      construit_n_grammes(i,
				  tableau_grames[i],
				  taille_sucesseur_1_grammes[i]);
        }
    }

    return model;
}


//Contrat de pré et poscondition de la fonction de construction du model de 1-grammes
model_N_grammes contrat_initialise_N_grammes(char* text,
					     int taille_text){

    assert(strlen(text) != 0);
    assert(strlen(text) != taille_text);
    assert(strlen(text) != taille_text);
    model_N_grammes resultat = initialise_N_grammes(text, taille_text);


    return resultat;
}



void test_model_un_gramme(){

    char* texte = "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux.";
    printf("taille texte = %ld \n",strlen(texte));
    printf("texte = %s \n",texte);

    int taille_text = 60;
    model_N_grammes model = initialise_N_grammes(texte,taille_text);
    
    N_grammes A_grammes = model.liste_un_gramme['a'];
    printf("%c gramme, sucesseur %c avec proba %d / %d \n",A_grammes.caractere, 
        A_grammes.successeur_grammes.sucesseur,
        A_grammes.successeur_grammes.occurence, A_grammes.occurence);

    N_grammes n_grammes = model.liste_un_gramme['n'];
    printf("%c gramme, sucesseur %c avec proba %d / %d \n",n_grammes.caractere, 
        n_grammes.successeur_grammes.sucesseur,
        n_grammes.successeur_grammes.occurence, n_grammes.occurence);

    N_grammes B_grammes = model.liste_un_gramme['B'];
    printf("%c gramme, sucesseur %c avec proba %d / %d \n",B_grammes.caractere, 
        B_grammes.successeur_grammes.sucesseur,
        B_grammes.successeur_grammes.occurence, B_grammes.occurence);

    assert((char)A_grammes.successeur_grammes.sucesseur == ' ');
    assert(n_grammes.successeur_grammes.sucesseur == 'j');

}


// ---- Prediction de la suite d'un text ---- 


// Prend en entrée un modéle de N-gramme, un text a complété et
//un entier qui donne la longeur total du text voulu
// Renvoi le text de longeur souhaité de telle sort que
//la suite du text est été prédite par le modéle

char* predicte_suivant(model_N_grammes m,
		       char* text,
		       int longeur_total){

    int longeur_actuelle = strlen(text);
    char text_complet[longeur_total];
    memcpy(text_complet,text,longeur_actuelle);

    for(int i=longeur_actuelle;i<longeur_total;i++){
        char caractere_suivant;
        if (m.liste_un_gramme[text_complet[i-1]].successeur_grammes.sucesseur != 0){
            caractere_suivant =
	      m.liste_un_gramme[text_complet[i-1]].successeur_grammes.sucesseur;
        } else {
            caractere_suivant = rand() % 256;
        }
        text_complet[i] = caractere_suivant;
    }
}


void test_predicte_suivant(){

    char* texte = "Bonjour, comment allez-vous ? Ca va, ca va aller bien .";
    int longeur_text = strlen(texte);

    model_N_grammes model = initialise_N_grammes(texte,longeur_text);

    char* resultat = predicte_suivant(model,texte,longeur_text+10);

    printf("Texte initial : \n %s\n",texte);

    printf("Texte complété : \n %s \n", resultat);
}


int main(){

  test_a_gramme_example();
  //test_model_un_gramme();
    //test_predicte_suivant();

    return 0;
}
