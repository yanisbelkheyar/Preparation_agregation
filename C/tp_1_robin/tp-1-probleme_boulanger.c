#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {
    Billet,
    PieceEuros,
    PieceCentimes,
    MAX_CASH_NATURE
} cash_nature_t;

typedef struct {
    cash_nature_t nature;
    unsigned valeur;
} cash_unite_t;

// Should really be a #define, but forbidden by agreg.
// Just using a const variable causes warnings from clang, as using a variable for an array size makes that array
// variable-sized (even if the variable is const, contrary to C++), and there are lots of places that a variable-sized
// array is illegal.
enum { NUM_CASH_UNITE = 7+2+6 };

const cash_unite_t Cash_unites[NUM_CASH_UNITE] = {
    {Billet, 500},
    {Billet, 200},
    {Billet, 100},
    {Billet, 50},
    {Billet, 20},
    {Billet, 10},
    {Billet, 5},
    {PieceEuros, 2},
    {PieceEuros, 1},
    {PieceCentimes, 50},
    {PieceCentimes, 20},
    {PieceCentimes, 10},
    {PieceCentimes, 5},
    {PieceCentimes, 2},
    {PieceCentimes, 1},
};

void print_output_line(const cash_unite_t *unite, unsigned nombre) {
    const char *nature_string_1_singulier[MAX_CASH_NATURE] = {"Billet", "Pièce", "Pièce"};
    const char *nature_string_1_pluriel[MAX_CASH_NATURE] = {"Billets", "Pièces", "Pièces"};
    const char *nature_string_2_singulier[MAX_CASH_NATURE] = {"Euro", "Euro", "Centime"};
    const char *nature_string_2_pluriel[MAX_CASH_NATURE] = {"Euros", "Euros", "Centimes"};

    const char *string1 = (nombre > 1 ? nature_string_1_pluriel : nature_string_1_singulier)[unite->nature];
    const char *string2 = (unite->valeur > 1 ? nature_string_2_pluriel : nature_string_2_singulier)[unite->nature];

    printf("%d %s de %d %s\n", nombre, string1, unite->valeur, string2);
}

void print_first_line(unsigned nombre_euros, unsigned nombre_centimes, unsigned nombre_billets_rendus, unsigned nombre_pieces_rendues) {
    printf("Rendre %d.%d %s nécessite d'utiliser %d %s et %d %s :\n",
        nombre_euros, nombre_centimes,
        (nombre_euros == 1) && (nombre_centimes == 0) ? "euro" : "euros",
        nombre_billets_rendus,
        nombre_billets_rendus > 1 ? "billets" : "billet",
        nombre_pieces_rendues,
        nombre_pieces_rendues > 1 ? "pieces" : "piece");
}

typedef struct {
    unsigned nombre_billets_rendus;
    unsigned nombre_pieces_rendues;
    unsigned rendus[NUM_CASH_UNITE];
} resultat_t;

void compute_rendus(int nombre_euros, int nombre_centimes, resultat_t *resultat) {
    for (size_t i = 0; i < NUM_CASH_UNITE; ++i) {
        int *valeur_a_rendre = (Cash_unites[i].nature == PieceCentimes ? &nombre_centimes : &nombre_euros);
        unsigned *nombre_a_accroitre = (Cash_unites[i].nature == Billet ? &resultat->nombre_billets_rendus : &resultat->nombre_pieces_rendues);

        int valeur_par_unite = Cash_unites[i].valeur;
        unsigned nombre_rendu = *valeur_a_rendre / valeur_par_unite;
        resultat->rendus[i] = nombre_rendu;
        *valeur_a_rendre -= nombre_rendu * valeur_par_unite;
        *nombre_a_accroitre += nombre_rendu;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments, expected 2, got %d\n", argc - 1);
        return 1;
    }

    int nombre_euros = atoi(argv[1]);
    int nombre_centimes = atoi(argv[2]);
    if (nombre_centimes >= 100) {
        nombre_euros += nombre_centimes / 100;
        nombre_centimes = nombre_centimes % 100;
        fprintf(stderr, "Plus de 100 centimes, ils sont convertis en euros\n");
    }

    resultat_t resultat = {0, 0, {0}};

    compute_rendus(nombre_euros, nombre_centimes, &resultat);

    print_first_line(nombre_euros, nombre_centimes, resultat.nombre_billets_rendus, resultat.nombre_pieces_rendues);
    for (size_t i = 0; i < NUM_CASH_UNITE; ++i) {
        if (resultat.rendus[i] > 0) {
            print_output_line(&Cash_unites[i], resultat.rendus[i]);
        }
    }
}