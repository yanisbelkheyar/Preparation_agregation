#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int Nb_lignes = 6;
const int Nb_colonnes = 7;
const char Jeton1 = 'X';
const char Jeton2 = 'O';
const char Case_vide = '_';

typedef enum {EN_COURS, VICTOIRE1, VICTOIRE2, MATCH_NUL} etat_partie_possible;
typedef enum {J1, J2} joueurs;
typedef enum {LIBRE, OCCUPE1, OCCUPE2} etat_case;

int nb_total_jetons = 0;

void print_case(etat_case my_case){
    switch (my_case){
        case LIBRE :
            printf("%c", Case_vide);
            break;
        case OCCUPE1 :
            printf("%c", Jeton1);
            break;
        case OCCUPE2 :
            printf("%c", Jeton2);
            break;
    }
}

void show_grill(etat_case (*grill)[Nb_colonnes]){
    printf("1234567\n\n");
    for (int i = 0; i<Nb_lignes; i++){
        for(int j = 0; j<Nb_colonnes; j++){
            print_case(grill[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

int ask_question(joueurs joueur, char *nom1, char *nom2){
    switch (joueur){
        case J1 :
            printf("%s (%c) : ", nom1, Jeton1);
            break;
        case J2 :
            printf("%s (%c) : ", nom2, Jeton2);
            break;
    }
    printf("quel est le numéro de la colonne dans laquelle vous souhaitez jouer un jeton ? ");
    int reponse;
    scanf("%d", &reponse);
    return reponse;
}

bool change_grill(etat_case (*grill)[Nb_colonnes], int colonne, joueurs joueur){
    //Prédoncition : colonne entre 1 et 7
    //Renvoie true si la colonne n'a pas pu etre remplie car pleine
    for(int i = Nb_lignes-1; i>=0; i--){
        if (grill[i][colonne-1] == LIBRE){
            if (joueur == J1) {
                grill[i][colonne-1] = OCCUPE1;
            }
            else {
                grill[i][colonne-1] = OCCUPE2;
            }
            break;
        }
        if (i==0){
            return true;
        }
    } 
    return false;
}

bool tester_gagnant(etat_case (*grill)[Nb_colonnes]){
    for(int i = 0; i<Nb_lignes; i++){
        for(int j = 0; j<Nb_colonnes; j++){
            etat_case my_case = grill[i][j];
            if (my_case==LIBRE){
                continue;
            }
            if(i<=2 && grill[i+1][j]==my_case && grill[i+2][j]==my_case && grill[i+3][j]==my_case){
                return true;
            }
            if(j<=3 && grill[i][j+1]==my_case && grill[i][j+2]==my_case && grill[i][j+3]==my_case){
                return true;
            }
            if(i<=2 && j<=3 && grill[i+1][j+1]==my_case && grill[i+2][j+2]==my_case && grill[i+3][j+3]==my_case){
                return true;
            }
            if(i<=2 && j>=3 && grill[i+1][j-1]==my_case && grill[i+2][j-2]==my_case && grill[i+3][j-3]==my_case){
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char **argv){
    char *nom1 = argv[1];
    char *nom2 = argv[2];
    printf("\n\n*** Bienvenue pour une nouvelle partie intankable de PUIIIIIIIISSAAAAAAAAAANCE 4 : %s vs %s***\n\n\n\n\n", nom1, nom2);
    etat_partie_possible etat_partie = EN_COURS;
    joueurs joueur_courant = J1;

    //Initialiser la grille
    etat_case grill[Nb_lignes][Nb_colonnes];
    for (int i = 0; i<Nb_lignes; i++){
        for(int j = 0; j<Nb_colonnes; j++){
            grill[i][j] = LIBRE;
        }
    }

    //Boucle de jeu
    int colonne_choisie = 0;
    while (etat_partie == EN_COURS){
        //afficher la grille
        show_grill(grill);
        //poser la question
        colonne_choisie = ask_question(joueur_courant, nom1, nom2);
        if (colonne_choisie<1 ||colonne_choisie>7){
            printf("Réponse invalide !\n");
            continue;
        }
        //changer la grille
        if(change_grill(grill, colonne_choisie, joueur_courant)){
            printf("Colonne déjà pleine !\n");
            continue;
        }; 
        //tester si quelqu'un a gagné
        if(tester_gagnant(grill)){
            if(joueur_courant==J1){
                etat_partie = VICTOIRE1;
            }
            else {
                etat_partie = VICTOIRE2;
            }
        }
        //incrémenter le nb de tour et changer de joueur
        joueur_courant = (joueur_courant+1)%2;
        nb_total_jetons++;
        //finir la partie s'il y a match nul
        if(nb_total_jetons==Nb_colonnes*Nb_lignes){
            etat_partie = MATCH_NUL;
        }
    }

    //montrer grille et dire qui a gagné
    show_grill(grill);
    switch (etat_partie){
        case MATCH_NUL :
            printf("\n\n\nStop, on arrête tout. Il y a...\n MATCH\n NUL !\n\n");
            break;
        case VICTOIRE1 :
            printf("\n\n\nLe vainqueur est... %s !\nBravo ! Bravo !\n\n",nom1);
            break;
        case VICTOIRE2 :
            printf("\n\n\nLe vainqueur est... %s !\nBravo ! Bravo !\n\n",nom2);
            break;
        default :
            printf("Problème rencontré je pense");
    }
}