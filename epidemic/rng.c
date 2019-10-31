#include "rng.h"

RNG* new_RNG(unsigned long seed) {
  RNG* self;
  self = (RNG*) malloc(sizeof(RNG));
  assert(self!=NULL);
  sgenrand(seed, &self->mt);
  return self;
}

void delete_RNG(RNG* self) {
  free(self);
}

double RNG_next_double(RNG* self) {
  return next_double(&self->mt);
}

unsigned long RNG_next_int(RNG* self) {
  return next_int(&self->mt);
}

double RNG_uniform(RNG* self) {
  return RNG_next_double(self);
}

double RNG_exponential(RNG* self, double lambda) {
  return -log(RNG_next_double(self))/lambda;
}

int RNG_randrange(RNG* self, int ind) {
    double index=RNG_uniform(self)*(double)ind;
    return floor(index)+1;
}
