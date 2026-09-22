#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//QUESTION 1
typedef struct successeurs_1_gramme{
    int occurences;
    char letter;
    struct successeurs_1_gramme *prochaine_successeur;
} successeurs_1_gramme ;

typedef struct clé_1_gramme{
    char letter;
    successeurs_1_gramme *premier_successeur;//liste chainée des successeurs, non triée
    struct clé_1_gramme *prochaine_clé;
} clé_1_gramme; 

typedef struct modele_1_gramme {
    clé_1_gramme *première_clé; //liste chainée des lettres vues
} modele_1_gramme;



//QUESTION 2
successeurs_1_gramme *créer_successeur(char successeur, successeurs_1_gramme *p){
    successeurs_1_gramme *nouveau_successeur = malloc(sizeof(successeurs_1_gramme));
    nouveau_successeur->occurences = 1;
    nouveau_successeur->letter = successeur;
    nouveau_successeur->prochaine_successeur = p;
    return nouveau_successeur;
}

void ajouter_successeur(clé_1_gramme *clé, char successeur){
    //On regarde si le successeur a déjà été vu au moins une fois
    successeurs_1_gramme *current_successor = clé->premier_successeur;
    while(current_successor!=NULL){
        //Si le successeur a été vu, on incrémente le nombre d'occurences.
        if(current_successor->letter == successeur){
            (current_successor->occurences)++;
            return;
        }
        else {
            current_successor = current_successor->prochaine_successeur;
        }
    }
    //Si le successeur n'a pas été vu, ajouter ce nouveau successeur avec une occurence de 1
    successeurs_1_gramme *nouveau_successeur = créer_successeur(successeur, clé->premier_successeur);
    clé->premier_successeur = nouveau_successeur;
    return;

}

clé_1_gramme *créer_clé_1_gramme(char letter, char successeur, clé_1_gramme *p){
    clé_1_gramme *nouvelle_clé = malloc(sizeof(clé_1_gramme));
    nouvelle_clé->letter = letter;
    successeurs_1_gramme *nouveau_successeur = créer_successeur(successeur, NULL);
    nouvelle_clé->premier_successeur = nouveau_successeur;
    nouvelle_clé->prochaine_clé = p;
    return nouvelle_clé;
}

void lire_lettre(modele_1_gramme *model, char lettre, char successeur){
    //Prends un modele 1 gramme, une lettre lue et son successeur et incrémente l'occurence de ce successeur pour cette lettre
    clé_1_gramme *current_key = model->première_clé;
    //On regarde si la lettre lue a déjà été vue, dans quel cas il suffit d'ajouter le successeur à la struct de la lettre
    while(current_key!=NULL){
        if(current_key->letter != lettre){
            current_key = current_key->prochaine_clé;
            continue;
        }
        else {
            ajouter_successeur(current_key, successeur);
            return;
        }
    }
    //Si c'est la première fois qu'on voit la lettre : on crée une nouvelle clé_1_gramme.
    clé_1_gramme *nouvelle_clé = créer_clé_1_gramme(lettre, successeur, model->première_clé);
    model->première_clé = nouvelle_clé;
}

modele_1_gramme *construire_modele(char *texte){
    modele_1_gramme *model = malloc(sizeof(modele_1_gramme));
    model->première_clé = NULL;
    while(*texte!='\0' && *(texte+1)!='\0'){
        lire_lettre(model, *texte, *(texte+1));
        texte++;
    }
    return model;
}

void print_clé(clé_1_gramme *clé){
    printf("\tLettre : %c\n", clé->letter);
    successeurs_1_gramme *p1 = clé->premier_successeur;
    while(p1!=NULL){
        printf("\t\tSuccesseur : %c ; Occurences : %i\n", p1->letter, p1->occurences);
        p1 = p1->prochaine_successeur;
    }
}

void print_model(modele_1_gramme *model){
    printf("-------------Printing a model.-------------\n");
    clé_1_gramme *p = model->première_clé;
    while(p!=NULL){
        print_clé(p);
        p = p->prochaine_clé;
    }
}

void free_clé(clé_1_gramme *clé){
    successeurs_1_gramme *p1 = clé->premier_successeur;
    while(p1!=NULL){
        successeurs_1_gramme *temp = p1->prochaine_successeur;
        free(p1);
        p1 = temp;
    }
}

void free_model(modele_1_gramme *model){
    clé_1_gramme *p = model->première_clé;
    while(p!=NULL){
        clé_1_gramme *temp = p->prochaine_clé;
        free_clé(p);
        free(p);
        p = temp;
    }
    free(model);
}



//QUESTION 3 
char get_last_char(char *texte){
    char c = '\0';
    while(*texte != '\0'){
        c = *(texte++);
    }
    return c;
}

char get_best_successor(successeurs_1_gramme *premier_successeur){
    int meilleur_occurence=0;
    int nb_meilleurs_candidats=0;
    successeurs_1_gramme *p = premier_successeur;
    //On regarde quelle est le successeur le plus fréquent et combien il y a de tels successeurs
    while(p!=NULL){
        int occ = p->occurences;
        if (occ == meilleur_occurence){
            nb_meilleurs_candidats++;
        }
        else if (occ > meilleur_occurence){
            meilleur_occurence = occ;
            nb_meilleurs_candidats=1;
        }
        p = p->prochaine_successeur;
    }
    //On tire si besoin l'un des meilleurs successeurs et on part à sa recherche
    int choix = rand() % nb_meilleurs_candidats;
    p = premier_successeur;
    while(p!=NULL){
        if(p->occurences == meilleur_occurence){
            if (choix == 0){
                return p->letter;
            }
            choix--;
        }
        p = p->prochaine_successeur;
    }
    return '\0'; //Ne devrait pas arriver
}

char get_next_char(modele_1_gramme *model, char c){
    /*Un step de la prédiction, renvoie le caractère deviné.*/
    clé_1_gramme *clé = model->première_clé;
    while(clé!=NULL){
        if(clé->letter != c){
            clé = clé->prochaine_clé;
            continue;
        }
        return get_best_successor(clé->premier_successeur);
    }
    //Si le modele ne sait pas quoi répondre : fin du texte répondu !
    return '\0';
}

char *prediction(modele_1_gramme *model, char *texte, int max_char ){
    /*Fonction centrale de la question 3*/

    /*On est en 1-gramme donc seulement le dernier caractère du texte d'entrée importe pour la prédiction*/
    char c = get_last_char(texte); 
    char *tab = malloc(sizeof(char)*max_char);
    int i;
    /*On prédit un à un les caractères en prenant la prédiction comme entrée de la prochaine prédiction.*/
    for(i = 0; i < max_char; i++){
        c = get_next_char(model, c);
        tab[i] = c;
        if (c=='\0'){
            return tab;
        }
    }
    tab[i+1] = '\0';
    return tab;
}

char *texte_entrainement = "Hello world... Je ne sais pas quoi mettre pour entrainer mon texte, pourtant il faudrait bien l'entrainer !! Alros il me faut trouver des choses a dire.";

int main(int argc, char **argv){
    srand(clock());
    modele_1_gramme *model = construire_modele(texte_entrainement);
    printf("Texte d'entrainement initial : %s\n\n", texte_entrainement);
    /*Prendre ' ' ou ',' comme exemple si on veut regarder un caractère en particulier (car central dans la prédiction).*/
    print_model(model);
    char *test_suite = "Bonjour,";
    char *texte_predit = prediction(model, test_suite, 100);
    printf("\n--------------------Complétion de texte :----------------\n%s\n%s\n", test_suite, texte_predit);
    free_model(model);
    free(texte_predit);
    return 0;
}