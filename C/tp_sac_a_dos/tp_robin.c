#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

long* vals;            //profits
long* poids;

void init_sad(int n, long pMax, long poids_div, long vals_div) {
    assert(poids_div < pMax);
    vals = (long*) calloc(n, sizeof(long));
    poids = (long*) calloc(n, sizeof(long));
    long a = 7;
    long b = 11;
    for(int i = 0 ; i < n ; i++){
        a = a * 7 % vals_div;
        b = b * 11 % poids_div;
        vals[i]  =  a + 1;
        poids[i] =  b + 1;
        assert(poids[i] <= pMax);
    }
}

void print_sad(int n, long pMax) {
    printf("Articles: %d, pMax=%ld:\nProfs:", n, pMax);
    for(int i=0; i < n; i++)
        printf("%5ld ",vals[i]);
    printf("\nPoids:");
    for(int i = 0; i < n; i++)
        printf("%5ld ",poids[i]);
    printf("\n");
}

typedef struct {
    size_t num_cols;
    size_t num_rows;
    long *vals;
} matrix;

size_t idx(const matrix *m, size_t row, size_t col) {
    return col*m->num_rows + row;
}

long get(const matrix *m, size_t row, size_t col) {
    return m->vals[idx(m, row, col)];
}

void set(const matrix *m, size_t row, size_t col, long v) {
    m->vals[idx(m, row, col)] = v;
}

// Initialize la matrice, avec des 0 dans chaque case.
void init_matrix(matrix *m, size_t num_rows, size_t num_cols) {
    m->num_cols = num_cols;
    m->num_rows = num_rows;
    m->vals = (long *) calloc(num_cols * num_rows, sizeof(long));
}

void swap (int *x, int*y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void items_pour_gloutons(int *items, int n) {
    for (int i = 0; i < n; ++i)
        items[i] = i;
    bool triFini = false;
    while (!triFini) {
        triFini = true;
        for (int i = 0; i < n-1; ++i) {
            int i1 = items[i];
            int i2 = items[i + 1];
            if (vals[i1] * poids[i2] < vals[i2] * poids[i1]) {
                triFini = false;
                swap(&items[i], &items[i+1]);
            }
        }
    }
}

// Algorithme glouton, avec tri par bulle
long calc_glouton(int n, long pMax) {
    // Init items to a sorted array of the items, by decreasing vals/poids
    int *items = (int *) malloc(sizeof(int) * n);
    items_pour_gloutons(items, n);

    long val = 0;
    long p = pMax; 
    for (int i = 0; i < n; ++i) {
        int item = items[i];
        if (poids[item] < p) {
            p -= poids[item];
            val += vals[item];
        }
    }
    free(items);
    return val;
}

long calc_valBorneSup(int n, long pMax) {
    // Init items to a sorted array of the items, by decreasing vals/poids
    int *items = (int *) malloc(sizeof(int) * n);
    items_pour_gloutons(items, n);

    long val = 0;
    long p = pMax;
    for (int i = 0; i < n && p > 0; ++i) {
        int item = items[i];
        p -= poids[item];
        val += vals[item];
    }
    free(items);
    return val;
}

long max(long a, long b) {
    return a < b ? b : a;
}

// m[i, p] est la meilleure valeur pour les items {0, .. i}, qui utilise *exactement* p poids.
long calc_val_dynamique_1(int n, long pMax) {
    matrix m;
    init_matrix(&m, n, pMax); // Initialise avec que des 0

    // TODO: essayer de mettre INT_MIN partout, et ne pas ajouter à ça.

    // Suffit à initialiser la première ligne
    if (poids[0] < pMax) {
        set(&m, 0, poids[0], vals[0]);
    }

    for (int i = 1; i < n; ++i) {
        for (int p = 1; p < pMax; ++p) {
            long val = get(&m, i-1, p);
            if (p >= poids[i]) {
                long val_selectione = get(&m, i-1, p-poids[i]) + vals[i];
                val = max(val, val_selectione);
            }
            set(&m, i, p, val);
        }
    }

    long result = 0;
    for (int p = 1; p < pMax; ++p) {
        long v = get(&m, n-1, p);
        result = max(v, result);
    }

    free(m.vals);
    return result;
}

// m[i, p] est la meilleure valeur pour les items {0, .. i} qui utilise *au plus* p poids
long calc_val_dynamique_2(int n, long pMax) {
    matrix m;
    init_matrix(&m, n, pMax);

    // Initialise la première ligne
    for (int p = poids[0]; p < pMax; p++) {
        set(&m, 0, p, vals[0]);
    }

    for (int i = 1; i < n; ++i) {
        for (int p = 1; p < pMax; ++p) {
            long val = max(get(&m, i-1, p), get(&m, i, p-1));
            if (p >= poids[i]) {
                long val_selectione = get(&m, i-1, p-poids[i]) + vals[i];
                val = max(val, val_selectione);
            }
            set(&m, i, p, val);
        }
    }

    long result = get(&m, n-1, pMax-1);
    free(m.vals);
    return result;
}

// Comme dynamique_1, mais avec une approche récursive qui évite de devoir calculer toutes les cases
long rec_aux(matrix *m, int i, int p) {
    assert(i >= 0);
    assert(p >= 0);
    long val = get(m, i, p);

    // val = 0 means not computed yet, val > 0 means computed and possible, val < 0 means impossible
    if (val > 0) return val;
    else if (val < 0) return INT_MIN;

    if (i == 0) {
        val = INT_MIN;
    } else if (p < poids[i]) {
        val = rec_aux(m, i-1, p);
    } else {
        val = max(rec_aux(m, i-1, p),
                  rec_aux(m, i-1, p-poids[i]) + vals[i]);
    }
    set(m, i, p, val);
    return val;
}

long calc_val_dynamique_rec(int n, long pMax) {
    matrix m;
    init_matrix(&m, n, pMax);

    // Suffit à initialiser la première ligne
    if (poids[0] < pMax) {
        set(&m, 0, poids[0], vals[0]);
    }

    long result = 0;
    for (int p = 1; p < pMax; ++p) {
        long v = rec_aux(&m, n-1, p);
        result = max(v, result);
    }

    free(m.vals);
    return result;
}

// Nouvelle idée: comme variante 1, mais avec une seule ligne.
// Et on le construit de façon un peu à l'envers (en profitant du fait qu'on le parcours dans le bon ordre)
// A chaque étape on le met à jour en ajoutant à chaque valeur > 0 ce qu'il faut si on peut sélectioner i
long calc_val_dynamique_une_ligne(int n, long pMax) {
    long *a = (void *) calloc(pMax, sizeof(long));

    if (poids[0] < pMax) {
        a[poids[0]] = vals[0];
    }
    for (int i = 1; i < n; ++i) {
        for (long p = 0; p < pMax; ++p) {
            if (a[p] == 0) continue;
            long new_p = p + poids[i];
            long new_v = a[p] + vals[i];
            if (new_p < pMax && a[new_p] < a[p] + vals[i]) {
                a[new_p] = new_v;
            }
        }
    }

    long resultat = INT_MIN;
    for (long p = 0; p < pMax; ++p) {
        resultat = max(resultat, a[p]);
    }
    free(a);
    return resultat;
}

typedef enum {
    dynamique_1,
    dynamique_2,
    dynamique_1_rec,
    dynamique_1_ligne
} algo;

void run_algo(algo algo, int n, long pMax) {
    long resultat;
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    switch (algo) {
        case dynamique_1:
            resultat = calc_val_dynamique_1(n, pMax);
            break;
        case dynamique_2:
            resultat = calc_val_dynamique_2(n, pMax);
            break;
        case dynamique_1_rec:
            resultat = calc_val_dynamique_rec(n, pMax);
            break;
        case dynamique_1_ligne:
            resultat = calc_val_dynamique_une_ligne(n, pMax);
            break;
    }
    gettimeofday(&stop, NULL);
    long num_ms = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
    printf("Resultat: %ld, nombre de microsecondes: %lu\n", resultat, num_ms);
}

int main(int argc, char** argv) {
    int n = 1000;
    long pMax = 60000;
    long poids_div = 5000;      // < pMax
    long vals_div  = 100;
    bool print_final_opt = false;
    if (argc < 2) {
        printf("Il faut donner au moins un argument pour sélectioner l'algorithme\n");
        return 1;
    }
    int algo_arg = atoi(argv[1]);
    algo algo;
    switch(algo_arg) {
        case 1: algo = dynamique_1; break;
        case 2: algo = dynamique_2; break;
        case 3: algo = dynamique_1_rec; break;
        case 4: algo = dynamique_1_ligne; break;
        default: {
            printf("Algo inconnu\n");
            return 2;
        }
    }
    if (argc == 7) {
        n = atoi(argv[2]);
        pMax = atoi(argv[3]);
        vals_div = atoi(argv[4]);
        poids_div = atoi(argv[5]);
        int a6 = atoi(argv[6]);
        if (a6 == 1) {
            print_final_opt = true;
        } else if (a6 != 0) {
            printf("print_final_opt est invalide\n");
            return 3;
        }
    }
    if (poids_div >= pMax) {
        printf("poids_div %ld n'est pas plus petit que pMax %ld\n", poids_div, pMax);
        return 4;
    }

    init_sad(n, pMax, poids_div, vals_div);
    if(print_final_opt)
        print_sad(n, pMax);

    long bsup = calc_valBorneSup(n, pMax);
    printf("bsup=%ld\n",bsup);

    run_algo(algo, n, pMax);
    return 0;
}
