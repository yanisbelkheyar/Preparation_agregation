#ifndef TAS_H
#define TAS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct tas_struct {
    int* tab;
    int length;
    int count;
} tas ;

tas creer(int l);
int get_min(tas t);
bool est_plein(tas t);

#endif