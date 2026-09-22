#include <stdio.h>
#include <stdlib.h>

typedef enum {BE, PE, PC} money;

const int Nb_billets = 7;
const int Nb_pieces_eur = 2;
const int Nb_pieces_cent = 6;

const int Tab_billets[] = {500, 200, 100, 50, 20, 10, 5};
const int Tab_pieces_eur[] = {2, 1};
const int Tab_pieces_cent[] = {50, 20, 10, 5, 2, 1};

int total_money(int montant, money type_argent, const int *tab){
    int i = 0;
    int num = 0;
    switch (type_argent) {
        case BE :
            while (montant > 0 && i < Nb_billets){
                int billet = tab[i++];
                int num_temp = montant/billet;
                if (num_temp == 1){
                    printf("1 billet de %i euros\n", billet);
                }
                else if (num_temp>1){
                    printf("%i billets de %i euros\n", num_temp, billet);
                }
                num += num_temp;
                montant = montant%billet;
            }
            break;
        case PE :
            while (montant > 0 && i < Nb_pieces_eur){
                int piece = tab[i++];
                int num_temp = montant/piece;
                if (num_temp == 1){
                    printf("1 pièce de %i euro", piece);
                    if(piece!=1){
                        printf("s\n");
                    }
                    else {
                        printf("\n");
                    }
                }
                else if (num_temp>1){
                    printf("%i pièces de %i euro", num_temp, piece);
                    if(piece!=1){
                        printf("s\n");
                    }
                    else {
                        printf("\n");
                    }
                }
                num += num_temp;
                montant = montant%piece;
            }
            break;
        case PC :
            while (montant > 0 && i < Nb_pieces_cent){
                int piece = tab[i++];
                int num_temp = montant/piece;
                if (num_temp == 1){
                    printf("1 pièce de %i centime", piece);
                    if(piece!=1){
                        printf("s\n");
                    }
                    else {
                        printf("\n");
                    }
                }
                else if (num_temp>1){
                    printf("%i pièces de %i centime", num_temp, piece);
                    if(piece!=1){
                        printf("s\n");
                    }
                    else {
                        printf("\n");
                    }
                }
                num += num_temp;
                montant = montant%piece;
            }
            break;
    }
    return num;
}

int main(int argc, char **argv){
    int euros = atoi(argv[1]);
    int centimes = atoi(argv[2]);
    int num_billets = total_money(euros, BE, Tab_billets);
    int num_pieces = total_money(euros%5, PE, Tab_pieces_eur);
    num_pieces += total_money(centimes, PC, Tab_pieces_cent);
    printf("\nPour un montant de %i.%i on a rendu %i billet", euros, centimes, num_billets);
    if(num_billets>1){
        printf("s");
    }
    printf(" et %i pièce", num_pieces);
    if(num_pieces>1){
        printf("s");
    }
    printf(".\n");
}