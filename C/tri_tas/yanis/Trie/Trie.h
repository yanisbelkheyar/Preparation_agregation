#include "../Tas/Trie_tas.h"
#include "../Trie_fusion/Trie_fusion.h"
#include "../Trie_rapide/Trie_rapide.h"

#include <enum.h>

typedef enum{
    Trie_rapide,
    Trie_fusion,
    Trie_tas,
} type_trie;


//Applique la fonction de trie indiqué a l'entrée donnée
int* Trie(int* tableau, type_trie fonction_trie,int taille_tableau); 