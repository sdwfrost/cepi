#include "simprocess.h"
#include <stdio.h>

SimProcess* new_SimProcess(Sim* my_sim) {
  SimProcess* self;
  self = (SimProcess*) malloc(sizeof(SimProcess));
  assert(self!=NULL);
  self->next_event_func=NULL;
  self->next_event_data=NULL;
  self->my_sim=my_sim;
  self->id=Sim_get_id(my_sim);
  self->is_activated=0;
  return self;
}

void delete_SimProcess(SimProcess* self) {
  /* (*self->next_event_freefunc)(data); */
  free(self);
}

void SimProcess_set_event(SimProcess* self, FunctionPointer func) {
  self->next_event_func=func;
  self->next_event_data=self;
}

void SimProcess_set_event_with_data(SimProcess* self, FunctionPointer func, void* data) {
  self->next_event_func=func;
  self->next_event_data=data;
}
