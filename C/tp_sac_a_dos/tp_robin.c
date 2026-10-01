#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    long val;
    long poids;
} item;

int itemComparator(const void *first, const void *second) {
    const item *i1 = first;
    const item *i2 = second;
    return (int) ((i2->val * i1->poids) - (i1->val * i2->poids));
}

item * init_sad(int n, long pMax, long poids_div, long vals_div) {
    assert(poids_div < pMax);
    item *items = (item *) calloc(n, sizeof(item));
    long a = 7;
    long b = 11;
    for(int i = 0 ; i < n ; i++) {
        a = a * 7 % vals_div;
        b = b * 11 % poids_div;
        items[i].val  =  a + 1;
        items[i].poids =  b + 1;
        assert(items[i].poids <= pMax);
    }
    return items;
}

void print_sad(int n, long pMax, item *items) {
    printf("Articles: %d, pMax=%ld:\nProfs:", n, pMax);
    for(int i=0; i < n; i++)
        printf("%5ld ", items[i].val);
    printf("\nPoids:");
    for(int i = 0; i < n; i++)
        printf("%5ld ", items[i].poids);
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

void swap (int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void tri_items_pour_gloutons(item *items, int n) {
    qsort(items, n, sizeof(item), itemComparator);
}

// Algorithme glouton, avec tri par bulle
long calc_glouton(int n, long pMax, item *items) {
    tri_items_pour_gloutons(items, n);

    long val = 0;
    long p = pMax; 
    for (int i = 0; i < n; ++i) {
        item item = items[i];
        if (item.poids < p) {
            p -= item.poids;
            val += item.val;
        }
    }
    return val;
}

long calc_valBorneSup(int n, long pMax, item *items) {
    tri_items_pour_gloutons(items, n);

    long val = 0;
    long p = pMax;
    for (int i = 0; i < n && p > 0; ++i) {
        item item = items[i];
        p -= item.poids;
        val += item.val;
    }
    return val;
}

long max(long a, long b) {
    return a < b ? b : a;
}

// Nouvelle idée: comme variante 1, mais avec une seule ligne.
// Et on le construit de façon un peu à l'envers (en profitant du fait qu'on le parcours dans le bon ordre)
// A chaque étape on le met à jour en ajoutant à chaque valeur > 0 ce qu'il faut si on peut sélectioner i
long calc_val_dynamique_une_ligne(int n, long pMax, const item *items) {
    long *a = (void *) calloc(pMax, sizeof(long));
    a[0] = 1; // cancelled out at the end
    for (int i = 0; i < n; ++i) {
        // Important de le parcourir dans cette direction pour ne pas ajouter un objet plusieurs fois !
        for (long p = pMax - 1; p >= 0; --p) {
            if (a[p] == 0) continue;
            long new_p = p + items[i].poids;
            long new_v = a[p] + items[i].val;
            if (new_p < pMax && a[new_p] < new_v) {
                a[new_p] = new_v;
            }
        }
    }

    long resultat = INT_MIN;
    for (long p = 0; p < pMax; ++p) {
        resultat = max(resultat, a[p]);
    }
    free(a);
    return resultat - 1; // -1 because of the 1 we started with
}

// Même idée que calc_val_dynamique_une_ligne, mais en utilisant un bitset pour marquer les cases non-nulles
// ... mais plus lent en pratique
void set_bit_true(uint64_t *bitset, long i) {
    bitset[i / 64] |= (1ULL << (i % 64));
}
void set_bit_false(uint64_t *bitset, long i) {
    bitset[i / 64] &= ~(1ULL << (i % 64));
}
long calc_val_bitset(int n, long pMax, item * items) {
    long *a = (void *) calloc(pMax, sizeof(long));
    long num_bitset_words = (pMax + 63) / 64;
    uint64_t *b = (void *) calloc(num_bitset_words, sizeof(uint64_t));

    set_bit_true(b, 0);    
    for (int i = 0; i < n; ++i) {
        for (long word_idx = num_bitset_words - 1; word_idx >= 0; --word_idx) {
            uint64_t w = b[word_idx];
            long p_base = 64 * (word_idx + 1) - 1;
            while (w) {
                int leading_zeroes = __builtin_clzll(w);
                long p =  p_base - leading_zeroes;
                assert(a[p] > 0 || p == 0);
                long new_p = p + items[i].poids;
                long new_v = a[p] + items[i].val;
                if (new_p < pMax && a[new_p] < new_v) {
                    a[new_p] = new_v;
                    set_bit_true(b, new_p);
                }
                // Deleting the top bit
                w ^= (1ULL<<63) >> leading_zeroes;
            }
        }
    }

    long resultat = INT_MIN;
    for (long p = 0; p < pMax; ++p) {
        resultat = max(resultat, a[p]);
    }
    free(b);
    free(a);
    return resultat;
}

// Même idée que dynamique_une_ligne, avec l'optimisation suivante: on commence par exécuter l'algo glouton pour une borne sup,
// et si on n'arrive pas à faire mieux que l'algo glouton on ne se fatigue pas à remplir une case.
long calc_val_dynamique_capped(int n, long pMax, item *items) {
    // Init items to a sorted array of the items, by decreasing vals/poids
    tri_items_pour_gloutons(items, n);
    // Even items a[2*p] contain the actual a[i, p] data for the current i
    // Odd items a[2*p+1] contain the lowest value that a path through that cell that leads to a better solution than the greedy one can have.
    // This is used to prune values that are worthless.
    long *a = (void *) malloc(2 * pMax * sizeof(long));

    long val = 0;
    long p = 0;
    long val_lower_bound = 0;
    for (int i = 0; i < n; ++i) {
        long new_p = p + items[i].poids;
        val += items[i].val;
        if (new_p < pMax) {
            for (int j = pMax - p - 1; j >= pMax - new_p; --j) {
                a[2*j+1] = -val;
            }
            p = new_p;
            val_lower_bound = val;
        } else {
            for (int j = pMax - p - 1; j >= 0; --j) {
                a[2*j+1] = -val;
            }
            break;
        }
    }
    for (long p = 0; p < pMax; ++p) {
        a[2*p+1] += val_lower_bound;
    }

    a[0] = 0;
    for (long p = 1; p < pMax; ++p) {
        a[2*p] = -1;
    }
    for (int i = n - 1; i >= 0; --i) {
        /*long countNonEmpty = 0;
        long countCapped = 0;
        long countSmaller = 0;
        long countBested = 0;
        long countAdded = 0;
        long last_v = INT_MAX;
        long last_p = INT_MAX;*/
        for (long p = pMax - items[i].poids - 1; p >= 0; --p) {
            long v = a[2*p];
            if (v == -1) continue;
            //countNonEmpty++;
            /*if (v > last_v) {
                countBested++;
                a[2*last_p] = -1;
            }
            last_p = p;
            last_v = v;*/
            long new_p = p + items[i].poids;
            long new_v = v + items[i].val;
            if (new_v < a[2*new_p+1]) {
                //countCapped++;
                continue;
            }
            if (new_v <= a[2*new_p]) {
                //countSmaller++;
                continue;
            }
            a[2*new_p] = new_v;
            //countAdded++;
        }
        //printf("i=%d, NonEmpty=%ld, Bested=%ld, Capped=%ld, Smaller=%ld, Added=%ld\n", i, countNonEmpty, countBested, countCapped, countSmaller, countAdded);
        /*if (i % 16 == 0) {
            long best = INT_MIN;
            for (long p = 0; p < pMax; ++p) {
                if (a[2*p] > best) best = a[2*p];
                else a[2*p] = -1;
            }
        }*/
    }

    long resultat = val_lower_bound;
    for (long p = 0; p < pMax; ++p) {
        resultat = max(resultat, a[2*p]);
    }
    free(a);
    return resultat;
}

/* TODO:
- Measure where we spend time !
- Make FOREACH_IN_BITSET_RIGHT_TO_LEFT take a highest idx allowed (would need looping on the first word apart from the rest)
- Then try the multi-level bitset idea
*/

#define MEASURE_TIME_FIRST(timeval_1) gettimeofday(&timeval_1, NULL);

#define MEASURE_TIME_NEXT(timeval_1, timeval_2, prefix_string) do { \
        gettimeofday(&timeval_2, NULL); \
        long num_ms = (timeval_2.tv_sec - timeval_1.tv_sec) * 1000000 + (timeval_2.tv_usec - timeval_1.tv_usec); \
        printf("%s: %lu us\n", prefix_string, num_ms); \
        timeval_1 = timeval_2; \
    } while(false)

#define FOREACH_IN_BITSET_RIGHT_TO_LEFT(bitset, num_words, prefix, idx, block) do {\
        for (long prefix##word_idx = num_words - 1; prefix##word_idx >= 0; --prefix##word_idx) {\
            uint64_t prefix##w = bitset[prefix##word_idx];\
            long prefix##idx_base = 64 * (prefix##word_idx + 1) - 1;\
            while(prefix##w) {\
                int prefix##leading_zeroes = __builtin_clzll(prefix##w);\
                prefix##w ^= (1ULL<<63) >> prefix##leading_zeroes;\
                idx = prefix##idx_base - prefix##leading_zeroes;\
                block\
            }\
        }\
    } while (false)

long calc_val_dynamique_capped_bitset(int n, long pMax, item *items) {
    struct timeval t1, t2;
    MEASURE_TIME_FIRST(t1);
    // Init items to a sorted array of the items, by decreasing vals/poids
    tri_items_pour_gloutons(items, n);
    MEASURE_TIME_NEXT(t1, t2, "Tri des objets");
    // TODO: update comment
    // Even items a[2*p] contain the actual a[i, p] data for the current i
    // Odd items a[2*p+1] contain the lowest value that a path through that cell that leads to a better solution than the greedy one can have.
    // This is used to prune values that are worthless.
    long *a = (void *) malloc(pMax * sizeof(long));

    long val = 0;
    long p = 0;
    long val_lower_bound = 0;
    // TODO: rather than this approach of putting it in the array, can't I just put the (new_p, val_bound) in a vector, and keep a cursor in it ?
    // It should nearly halve the memory usage, thus improving the cache locality. And it would be a pre-requisite to the tree of bitset approach (or at least to make it a big win).
    for (int i = 0; i < n; ++i) {
        long new_p = p + items[i].poids;
        val += items[i].val;
        if (new_p < pMax) {
            for (int j = pMax - p - 1; j >= pMax - new_p; --j) {
                a[j] = -val;
            }
            p = new_p;
            val_lower_bound = val;
        } else {
            for (int j = pMax - p - 1; j >= 0; --j) {
                a[j] = -val;
            }
            break;
        }
    }
    for (long p = 0; p < pMax; ++p) {
        // -1 so we can use <= rather than <, which unifies it with another check
        a[p] += val_lower_bound - 1;
    }
    MEASURE_TIME_NEXT(t1, t2, "Calcul des bornes");

    long num_bitset_words = (pMax + 63) / 64;
    uint64_t *b = (void *) calloc(num_bitset_words, sizeof(uint64_t));

    set_bit_true(b, 0);
    a[0] = 0;
    for (int i = n - 1; i >= 0; --i) {
        // long last_v = INT_MAX;
        // long last_p;
        FOREACH_IN_BITSET_RIGHT_TO_LEFT(b, num_bitset_words, _b_, p,
            long v = a[p];
            assert(v > 0 || p == 0);
            // Version branch-free de if(v >= last_v) set_bit_false(b, last_p)
            // La version non-branch-free rend le code un peu plus lent, celle-ci le rend un peu plus rapide (50ms vs 47ms vs 48ms sur les params 3000 200000 100 50000)
            /* b[last_p / 64] &= v >= last_v ? ~(1ULL << (last_p % 64)) : ~0ULL;
            printf("A\n");
            last_v = v;
            last_p = p;*/
            long new_p = p + items[i].poids;
            if (new_p >= pMax) {
                continue;
            }
            long new_v = v + items[i].val;
            if (new_v <= a[new_p]) {
                continue;
            }
            a[new_p] = new_v;
            set_bit_true(b, new_p);
        );
        if (i % 100 == 0) {
            MEASURE_TIME_NEXT(t1, t2, "Dans la boucle principale");
        }
    }

    long resultat = INT_MIN;
    FOREACH_IN_BITSET_RIGHT_TO_LEFT(b, num_bitset_words, _b_, p,
        resultat = max(resultat, a[p]);
    );
    free(b);
    free(a);
    MEASURE_TIME_NEXT(t1, t2, "Récupération des résultats");
    return resultat;
}

long calc_val_dynamique_per_value(int n, long pMax, const item *items, long borne_sup) {
    /* a[v] is either
     * pMax if it is impossible to achieve v value with the items considered so far
     * p if that is the smallest weight required to achieve v with the items considered so far
     */
    long *a = malloc(borne_sup * sizeof(long));
    for (long v = 1; v < borne_sup; ++v) {
        a[v] = pMax;
    }
    a[0] = 0;

    for (int i = 0; i < n; ++i) {
        // Important de le parcourir dans ce sens pour ne pas ajouter un objet plusieurs fois !
        for (long v = borne_sup - items[i].val - 1; v >= 0; --v) {
            long new_v = v + items[i].val;
            long new_p = a[v] + items[i].poids;
            if (a[new_v] > new_p) {
                a[new_v] = new_p;
            }
        }
    }

    long resultat = borne_sup;
    for (long v = borne_sup - 1; v >= 0; --v) {
        if (a[v] < pMax) {
            resultat = v;
            return resultat;
        }
    }
    assert(false);
}

typedef struct {
    int n;
    long pMax;
    const item *items;
    long borne_sup;
    long *result;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
} concurrent_args;

void * per_weight_concurrent_helper(void *arg) {
    concurrent_args *true_args = (concurrent_args *) arg;
    long tmp = calc_val_dynamique_une_ligne(true_args->n, true_args->pMax, true_args->items);
    pthread_mutex_lock(true_args->lock);
    *true_args->result = tmp;
    pthread_cond_signal(true_args->cond);
    pthread_mutex_unlock(true_args->lock);
    return NULL;
}

void * per_value_concurrent_helper(void *arg) {
    concurrent_args *true_args = (concurrent_args *) arg;
    long tmp = calc_val_dynamique_per_value(true_args->n, true_args->pMax, true_args->items, true_args->borne_sup);
    pthread_mutex_lock(true_args->lock);
    *true_args->result = tmp;
    pthread_cond_signal(true_args->cond);
    pthread_mutex_unlock(true_args->lock);
    return NULL;
}

long calc_val_concurrent_value_weight(int n, long pMax, const item *items, long borne_sup) {
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_t cond;
    pthread_cond_init(&cond, NULL);
    long result[2] = {INT_MIN, INT_MIN};
    pthread_t fils[2];
    concurrent_args args[2] = {{n, pMax, items, borne_sup, &result[0], &lock, &cond}, {n, pMax, items, borne_sup, &result[1], &lock, &cond}};
    pthread_create(&fils[0], NULL, per_weight_concurrent_helper, &args[0]);
    pthread_create(&fils[1], NULL, per_value_concurrent_helper, &args[1]);
    pthread_mutex_lock(&lock);
    while (result[0] == INT_MIN && result[1] == INT_MIN) {
        pthread_cond_wait(&cond, &lock);
    }
    long true_result = max(result[0], result[1]);
    pthread_mutex_unlock(&lock);
    pthread_cancel(fils[0]);
    pthread_cancel(fils[1]);
    pthread_join(fils[0], NULL);
    pthread_join(fils[1], NULL);
    return true_result;
}

typedef enum {
    dynamique_ligne,
    dynamique_bitset,
    dynamique_capped,
    dynamique_capped_bitset,
    dynamique_per_value,
    concurrent_value_weight,
    ALGO_MAX
} algo;

const char *algo_to_string[ALGO_MAX] = {
    "[1] Dynamique 1 (alloue une seule ligne)",
    "[2] Dynamique 1 (alloue une seule ligne, et utilise un bitset pour skipper les cases vides)",
    "[3] Dynamique 1 (alloue une seule ligne, utilise un algo glouton pour éliminer les cas inutile)",
    "[4] Dynamique 1 (alloue une seule ligne, elimine les cas inutile, et utilise un bitset pour iterer les cases utiles)",
    "[5] Dynamique 1 (alloue une seule ligne, par valeur plutôt que par poids)",
    "[6] 1 et 5 en parallèle, renvoie un résultat dès qu'un a fini"
};

void run_algo(algo algo, int n, long pMax, item * items) {
    long resultat;
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    switch (algo) {
        case dynamique_ligne:
            resultat = calc_val_dynamique_une_ligne(n, pMax, items);
            break;
        case dynamique_bitset:
            resultat = calc_val_bitset(n, pMax, items);
            break;
        case dynamique_capped:
            resultat = calc_val_dynamique_capped(n, pMax, items);
            break;
        case dynamique_capped_bitset:
            resultat = calc_val_dynamique_capped_bitset(n, pMax, items);
            break;
        case dynamique_per_value: {
            long borne_sup = calc_valBorneSup(n, pMax, items);
            resultat = calc_val_dynamique_per_value(n, pMax, items, borne_sup);
            break;
        }
        case concurrent_value_weight: {
            long borne_sup = calc_valBorneSup(n, pMax, items);
            resultat = calc_val_concurrent_value_weight(n, pMax, items, borne_sup);
            break;
        }
        case ALGO_MAX: assert(false);
    }
    gettimeofday(&stop, NULL);
    long num_ms = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
    printf("Algo: %s\n", algo_to_string[algo]);
    printf("Resultat: %ld, nombre de microsecondes: %lu\n", resultat, num_ms);
}

int main(int argc, char** argv) {
    int n = 1000;
    long pMax = 60000;
    long vals_div  = 100;
    long poids_div = 5000;      // < pMax
    bool print_final_opt = false;
    if (argc < 2) {
        printf("Il faut donner au moins un argument pour sélectioner l'algorithme\n");
        return 1;
    }
    int algo_arg = atoi(argv[1]);
    algo algo;
    switch(algo_arg) {
        case 1: algo = dynamique_ligne; break;
        case 2: algo = dynamique_bitset; break;
        case 3: algo = dynamique_capped; break;
        case 4: algo = dynamique_capped_bitset; break;
        case 5: algo = dynamique_per_value; break;
        case 6: algo = concurrent_value_weight; break;
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
    } else if (argc != 2) {
        printf("Usage: algo n pMax vals_div poids_div print\n");
        return 4;
    }
    if (poids_div >= pMax) {
        printf("poids_div %ld n'est pas plus petit que pMax %ld\n", poids_div, pMax);
        return 5;
    }

    item *items = init_sad(n, pMax, poids_div, vals_div);
    pMax = pMax + 1; // Je n'avais pas réalisé que c'est une borne inclusive plutôt que exclusive.
    if (print_final_opt)
        print_sad(n, pMax, items);

    long bsup = calc_valBorneSup(n, pMax, items);
    printf("bsup=%ld\n",bsup);

    run_algo(algo, n, pMax, items);
    free(items);
    return 0;
}
