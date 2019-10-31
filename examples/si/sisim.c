#include <assert.h>
#include "sisim.h"
#include "siperson.h"

SiSim* new_SiSim(int id, double start_time, double stop_time, unsigned long seed, int capacity) {
  SiSim* self;
  self = (SiSim*) malloc(sizeof(SiSim));
  assert(self!=NULL);
   /* Times */
  self->start_time=start_time;
  self->stop_time=stop_time;
  self->current_time=start_time;
  /* IDs */
  self->id=id;
  self->current_id=0;
  /* Initialize structs */
  self->rng=new_RNG(seed);
  self->evlist=new_EventList();
  self->procarray=new_Array(capacity);
  /* Set flags */
  self->run_flag=0;
  self->pause_flag=0;
  /* Parameters */
  self->inf_prob=0.0;
  self->contact_rate=0.0;
  /* States */
  self->num_infected=0;
  return self;
}

void delete_SiSim(SiSim* self) {
     int i;
     delete_RNG(self->rng);
     delete_EventList(self->evlist);
     for(i=0;i<(self->procarray->pos+1);i++){
          delete_SiPerson(Array_at(self->procarray,i));
     }
     delete_Array(self->procarray);
     free(self);
}

void SiSim_initialize(SiSim* self, double inf_prob, double contact_rate) {
     self->inf_prob=inf_prob;
     self->contact_rate=contact_rate;
}

void SiSim_activate(SiSim* self, int popsize, int num_infected) {
     int index;
     SiPerson* pers;
     for (index = 0; index < (popsize-num_infected); ++index) {
         pers=new_SiPerson(self,1);
         SiPerson_activate(pers);
         Array_append(self->procarray,(void*)pers);
     }
     for (index = (popsize-num_infected); index < popsize; ++index) {
         pers=new_SiPerson(self,2);
         SiPerson_activate(pers);
         Array_append(self->procarray,(void*)pers);
         self->num_infected=self->num_infected+1;
     }
}     

