#include <assert.h>
#include "siperson.h"

SiPerson* new_SiPerson(SiSim* my_sim, int state) {
  SiPerson* self;
  self = (SiPerson*)malloc(sizeof(SiPerson));
  assert(self!=NULL);
  self->next_event_func=NULL;
  self->next_event_data=NULL;
  self->my_sim=my_sim;
  self->is_activated=1;
  self->id=Sim_get_id((Sim*)my_sim);
  self->state=state;
  return self;
}

void delete_SiPerson(SiPerson* self) {
     free(self);
}
     
double SiPerson_contact_interval(SiPerson* self) {
     return RNG_exponential(self->my_sim->rng,self->my_sim->contact_rate);
}

SiPerson* SiPerson_choose_contact(SiPerson* self) {
     int index;
     SiPerson* person_at_index;
     index=RNG_randrange(self->my_sim->rng,self->my_sim->procarray->pos)-1;
     person_at_index=(SiPerson*)Sim_get_process((Sim*)self->my_sim,index);
     while (self == person_at_index){
           index=RNG_randrange(self->my_sim->rng,self->my_sim->procarray->pos)-1;
           person_at_index=(SiPerson*)Sim_get_process((Sim*)self->my_sim,index);
     }
     return person_at_index;
}

void SiPerson_activate(SiPerson* self) {
     double ci;
     if(self->state==1){
           ci=SiPerson_contact_interval(self);
           SimProcess_set_event((SimProcess*)self,(FunctionPointer)SiPerson_live);
           Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
           self->is_activated=1;
     }
}

void SiPerson_live(SiPerson* self) {
     double ci;
     SiPerson* B;
     if(self->state==1){
            B=SiPerson_choose_contact(self);
            if(B->state==2){
                 double x;
                 x=RNG_uniform(self->my_sim->rng);
                 if(x<self->my_sim->inf_prob){
                     self->my_sim->num_infected=self->my_sim->num_infected+1;
                     /* printf("%f%s%i\n",self->my_sim->current_time,"\t",self->my_sim->num_infected); */
                     printf("%i\t%4.12f\t%i\t%i\n",self->my_sim->id,self->my_sim->current_time,B->id,self->id);
                     self->state=2;
                     }
                 else{
                      ci=SiPerson_contact_interval(self);
                      Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
                 }
            }
            else{
                 ci=SiPerson_contact_interval(self);
                 Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
            }
     }
}

