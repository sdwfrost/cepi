#include "array.h"
#include <stdio.h>

Array* new_Array(int capacity) {
  Array* self;
  self = (Array*) malloc(sizeof(Array));
  assert(self!=NULL); 
  self->pos=0;
  self->capacity=capacity;
  self->items=NULL;
  self->items=(void**)realloc(self->items,self->capacity*sizeof(size_t));
  return self;
}

void Array_append(Array* self, void* item) {
  if(self->pos==self->capacity){
     self->capacity=self->capacity*2;
     self->items=(void**)realloc(self->items,self->capacity*sizeof(size_t));                    
  }
  self->items[self->pos++]=item;
}

void* Array_at(Array* self, int pos) {
  return self->items[pos];
}

void delete_Array(Array* self) {
  free(self->items);
  free(self);
}
