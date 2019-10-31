#include "sim.h"

Sim* new_Sim(int id, double start_time, double stop_time, unsigned long seed, int capacity) {
  Sim* self;
  self = (Sim*) malloc(sizeof(Sim));
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
  /* TO DO: Set parameters and states */
  return self;
}

void delete_Sim(Sim* self) {
     int i;
     delete_RNG(self->rng);
     delete_EventList(self->evlist);
     for(i=0;i<(self->procarray->pos+1);i++){
          delete_SimProcess(Array_at(self->procarray,i));
     }
     delete_Array(self->procarray);
     free(self);
}

void Sim_schedule(Sim* self, double t, SimProcess* p) {
  EventList_insert(self->evlist,t,p);
}

void Sim_append(Sim* self, SimProcess* p) {
  Array_append(self->procarray,(void*)p);
}

SimProcess* Sim_get_process(Sim* self, int index) {
  return (SimProcess*)Array_at(self->procarray,index);
}

int Sim_get_id(Sim* self){
  int new_id=self->current_id;
  self->current_id=self->current_id+1;
  return new_id;
}

void Sim_do_all(Sim* self){
  while((self->evlist->count>0) &&(self->current_time<self->stop_time)){
    Sim_step(self);
  }
}

void Sim_do_until_pause(Sim* self){
  while((self->evlist->count>0)&&(self->pause_flag==0)){
    Sim_step(self);
  }
}

void Sim_step_for_interval(Sim* self, double interval){
  double nexttime;
  nexttime=self->current_time+interval;
  while((self->evlist->count>0)&&(self->current_time<nexttime)){
    Sim_step(self);
  }
}

void Sim_step(Sim* self){
  EventList_next(self->evlist);
  if (self->evlist->count>=0)
  {
      self->current_time=self->evlist->t;
      (*self->evlist->p->next_event_func)(self->evlist->p->next_event_data);
  }
}
                               
void Sim_stop(Sim* self){
   self->run_flag=0;
}   
