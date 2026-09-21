#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const int MAX_ERREURS = 6;

/*Renvoie la longueur du string donné en entrée*/
int get_length(char *s){
    int i=0;
    while(*s != '\0'){
        i++;
        s++;
    }
    return i;
}

/*Choisie un mot aléatoire du fichier passé en argumant*/
char *choose_word(char *filename){
    int num_mots;
    FILE *f = fopen(filename, "r");
    fscanf(f, "%d", &num_mots);
    srand(time(NULL));
    int chosen_num = rand() % num_mots;
    char poubelle[100];
    for(int i=0; i<=chosen_num;i++){
        fscanf(f, "%s", poubelle);
    }
    fclose(f);
    char *rep = malloc(sizeof(char)*(get_length(poubelle)+1));
    for(int i =0; i<get_length(poubelle);i++){
        rep[i] = poubelle[i];
    }
    rep[get_length(poubelle)] = '\0';
    return rep;
}

/*True si le charactère a déjà été guess par l'utilisateur. False sinon.*/
bool already_guessed(char *guessed, int num_guess, char c){
    for(int i=0; i<num_guess; i++){
        if(guessed[i]==c){
            return true;
        }
    }
    return false;
}

/*Vérifie que le caractère est dans a-z A-Z ou -*/
bool is_valid_char(char c){
    return ((c>='a' && c<='z') || (c>='A'&&c<='Z'||c=='-'));
}

/*Boucle principale*/
void gameloop(char *word, int n){
    int found[n];
    for(int i = 0; i<n; i++)found[i]=0;
    char guessed[50];
    int num_guess = 0;
    int wrong_guesses = 0;
    int correct_guesses = 0;
    /*Entrée de la boucle principale*/
    while(wrong_guesses<MAX_ERREURS && correct_guesses<n){
        char c;
        bool guessed_flag = false;
        bool incorrect_input_flag = false;
        /*Chopper le guess*/
        do{
            do{
                /*Afficher les éventuels messages d'erreur d'input.*/
                if(guessed_flag){
                    printf("\nLettre déjà devinée !\n");
                    guessed_flag = false;
                }
                if(incorrect_input_flag){
                    printf("\nInput invalide ! Entrer un truc a-z, A-Z ou -.\n");
                    incorrect_input_flag = false;
                }
                /*Afficher mot, lettres saisies, nb essais restants et poser question*/
                printf("\nMot : ");
                for(int i =0; i<n; i++){
                    if(found[i]==0){
                        printf("_");
                    }
                    else{
                        printf("%c",word[i]);
                    }
                }
                printf("\nLettres saisies : ");
                for(int i=0; i<num_guess;i++){
                    printf("%c",guessed[i]);
                }
                printf("\nNombre d'essais restants : %i", MAX_ERREURS-wrong_guesses);
                printf("\nEntrer une lettre : ");
                scanf("%c", &c);
                int i; while ((i = getchar()) != '\n' && i != EOF);
                incorrect_input_flag = true;
            }while(!is_valid_char(c));
            incorrect_input_flag = false;
            guessed_flag = true;
        }while(already_guessed(guessed, num_guess, c));
        guessed[num_guess]=c;
        num_guess++;
        /*Check if guess is right or wrong and add found or incr wrong_guess*/
        bool is_correct_guess = false;
        for(int i=0; i<n; i++){
            if(word[i]==c){
                found[i] =1;
                is_correct_guess = true;
                correct_guesses++;
            }
        }
        if(!is_correct_guess){
            wrong_guesses+=1;
            printf("\nDommage ! Mauvais guess !\n\n");
        }
        else{
            printf("\nBravo ! Bon guess !\n\n");
        }
    }
    /*Dire si l'utilisateur a gagné ou perdu*/
    if(correct_guesses==n){
        printf("\n\n\n\nBRAVO !!!!!!! Tu es absoluement GOATESQUE ! Le mot était bien : %s\n\n\n\n", word);
    }
    else{
        printf("\n\n\n\nLOSER ! Tu t'es fait mog bêtement ! Le mot était : %s\n\n\n\n", word);
    }
}

int main(int argc, char **argv){
    if(argc<2){
        printf("Syntaxe : ./exercice2 [nom_du_fichier_de_donnees]\n");
        exit(0);
    }
    char *nom_fichier=argv[1];
    char *mot = choose_word(nom_fichier);
    int n = get_length(mot);
    printf("Vous avez %i essais, a vous de jouer...\n\n",MAX_ERREURS);
    gameloop(mot,n);
    free(mot);
}