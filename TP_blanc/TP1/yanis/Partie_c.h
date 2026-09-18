//structure représentant un sucesseur de 1-grammes et son nombre d'occurence
typedef struct {
    char sucesseur;
    int occurence;
} Suivant;

//structure contenant un 1-grammes
typedef struct {
    char caractere;             // caractére représentant le 1-gramme
    Suivant successeur_grammes;   // structure contenant les sucesseurs possible et leur occurences
    int occurence;                   // nombre total d'occurence du 1-grammes       
} N_grammes;  


//structure contenant l'ensemble des 1-grammes
typedef struct {
    N_grammes liste_un_gramme [256];
} model_N_grammes;


N_grammes construit_n_grammes(char caractere, char* sucesseur, int nombre_occurence_1_gramme);

model_N_grammes initialise_N_grammes(char* text, int taille_text);

char* predicte_suivant(model_N_grammes m, char* text, int longeur_total);
