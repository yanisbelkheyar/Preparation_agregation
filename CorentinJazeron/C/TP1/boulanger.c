#include <stdio.h>
#include <stdlib.h>

enum monnaie_type {BE, PE, PC};
typedef enum monnaie_type monnaie;

const int Nb_valeurs_be = 7;
const int Nb_valeurs_pe = 2;
const int Nb_valeurs_pc = 6;

int valeurs_be[7] = {500, 200, 100, 50, 20, 10, 5};
int valeurs_pe[2] = {2, 1};
int valeurs_pc[6] = {50, 20, 10, 5, 2, 1};

int tab_be_rendus[7] = {0};
int tab_pe_rendus[2] = {0};
int tab_pc_rendus[6] = {0};

int nombre_monnaie(int* montant_res, monnaie t, int* tab_rendus) {
	int sum = 0;
	int* tab_val;
	int tab_len;
	switch (t) {
    case BE:
    	tab_len = Nb_valeurs_be;
    	tab_val = valeurs_be;
        break;
    case PE:
    	tab_len = Nb_valeurs_pe;
    	tab_val = valeurs_pe;
        break;
    case PC:
    	tab_len = Nb_valeurs_pc;
    	tab_val = valeurs_pc;
        break;
	}
	for (int i = 0; i < tab_len; ++i) {
		while (*montant_res >= tab_val[i]) {
			++tab_rendus[i];
			++sum;
			*montant_res -= tab_val[i];
		}
	}
	return sum;
}

void print_res(monnaie t, int* tab_rendus) {
	int* tab_val;
	int tab_len;
	char* nom;
	char* nom2;
	switch (t) {
	case BE:
    	tab_len = Nb_valeurs_be;
    	tab_val = valeurs_be;
    	nom = "billet";
    	nom2 = "euro";
        break;
    case PE:
    	tab_len = Nb_valeurs_pe;
    	tab_val = valeurs_pe;
    	nom = "pièce";
    	nom2 = "euro";
        break;
    case PC:
    	tab_len = Nb_valeurs_pc;
    	tab_val = valeurs_pc;
    	nom = "piece";
    	nom2 = "centime";
        break;
	}

	char s_euro;
	for (int i = 0; i < tab_len; ++i) {
		if (tab_val[i] > 1) {
					s_euro = 's';
				} else {
					s_euro = '\0';
				}
		switch (tab_rendus[i]) {
			case 0:
				break;
			case 1:
				printf("%d %s de %d %s%c.\n", tab_rendus[i], nom, tab_val[i], nom2, s_euro);
				break;
			case 2:
				printf("%d %ss de %d %s.\n", tab_rendus[i], nom, tab_val[i], nom2, s_euro);
				break;
		}
	}
}

void print_headline(const char* montant_u, const char* montant_c, char s_montant, int sum_be, int sum_pe, int sum_pc) {
	printf("Rendre %s.%s euro%c ", montant_u, montant_c, s_montant);
	switch(sum_be) {
		case 0:
			switch(sum_pe + sum_pc) {
				case 0:
					printf("ne nécessite rien.\n");
					break;
				case 1:
					printf("nécessite %d pièce.\n", sum_pe + sum_pc);
					break;
				default:
					printf("nécessite %d pièces.\n", sum_pe + sum_pc);
					break;
			}
			break;
		case 1:
			switch(sum_pe + sum_pc) {
				case 0:
					printf("nécessite %d billet.\n", sum_be);
					break;
				case 1:
					printf("nécessite %d billet et %d pièce.\n", sum_be, sum_pe + sum_pc);
					break;
				default:
					printf("nécessite %d billet et %d pièces.\n", sum_be, sum_pe + sum_pc);
					break;
			}
			break;
		default:
			switch(sum_pe + sum_pc) {
				case 0:
					printf("nécessite %d billets.\n", sum_be);
					break;
				case 1:
					printf("nécessite %d billets et %d pièce.\n", sum_be, sum_pe + sum_pc);
					break;
				default:
					printf("nécessite %d billets et %d pièces.\n", sum_be, sum_pe + sum_pc);
					break;
			}
			break;
	}
}

int main(int argc, char const *argv[])
{
	int montant_unit = atoi(argv[1]);
	int montant_cents = atoi(argv[2]);
	char s_montant = '\0';
	if ((montant_unit > 1) || (montant_unit == 1 && montant_cents > 0)) {
		s_montant = 's';
	}

	int sum_be = nombre_monnaie(&montant_unit, BE, tab_be_rendus);
	int sum_pe = nombre_monnaie(&montant_unit, PE, tab_pe_rendus);
	int sum_pc = nombre_monnaie(&montant_cents, PC, tab_pc_rendus);

	print_headline(argv[1], argv[2], s_montant, sum_be, sum_pe, sum_pc);
	print_res(BE, tab_be_rendus);
	print_res(PE, tab_pe_rendus);
	print_res(PC, tab_pc_rendus);

	return 0;
}