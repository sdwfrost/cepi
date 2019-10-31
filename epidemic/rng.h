#ifndef _RNG_H
#define _RNG_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "mt19937p.h"

typedef struct RNG {
    struct mt19937p mt;
} RNG;

RNG* new_RNG(unsigned long seed);
void delete_RNG(RNG* self);
double RNG_next_double(RNG* self);
unsigned long RNG_next_int(RNG* self);
double RNG_uniform(RNG* self);
double RNG_exponential(RNG* self, double lambda);
int RNG_randrange(RNG* self, int ind);

#endif
