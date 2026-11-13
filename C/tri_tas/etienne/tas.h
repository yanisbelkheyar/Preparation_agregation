#ifndef TAS_H
#define TAS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <limits.h>

typedef struct tas_struct {
    int* tab;
    int length;
    int count;
} tas ;

tas creer(int l);
int get_min(tas* t);
bool est_plein(tas* t);
bool est_vide(tas* t);
void print(tas* t);
bool inserer(tas* t,int val);
int supprime_min(tas* t);
void tris_tas(int* tab, int l);

#endif