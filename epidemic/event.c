#include "event.h"
#include <stdio.h>

Event* new_Event(double t, void* item) {
  Event* self;
  self = (Event*) malloc(sizeof(Event));
  assert(self!=NULL);
  self->t = t;
  self->item = item;
  return self;
}

void delete_Event(Event* self){
  free(self);
}
      
int compare_times(const void *event_a, const void *event_b, void * avl_param)
{
    double t1 = ((Event*)event_a)->t;
    double t2 = ((Event*)event_b)->t;
    
    if (t1 > t2) return 1;
    if (t1 < t2) return -1;
    return 0;
}
