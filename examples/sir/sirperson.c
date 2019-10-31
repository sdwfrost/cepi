#include <assert.h>
#include "sirperson.h"

SirPerson* new_SirPerson(SirSim* my_sim, int state) {
  SirPerson* self;
  self = (SirPerson*) malloc(sizeof(SirPerson));
  assert(self!=NULL);
  self->next_event_func=NULL;
  self->next_event_data=NULL;
  self->is_activated=0;
  self->my_sim=my_sim;
  self->id=Sim_get_id((Sim*)my_sim);
  self->state=state;
  return self;
}

void delete_SirPerson(SirPerson* self) {
     free(self);
}
     
double SirPerson_contact_interval(SirPerson* self) {
     return RNG_exponential(self->my_sim->rng,self->my_sim->contact_rate);
}

double SirPerson_recovery_interval(SirPerson* self) {
     return RNG_exponential(self->my_sim->rng,self->my_sim->recovery_rate);
}

SirPerson* SirPerson_choose_contact(SirPerson* self) {
     int index;
     SirPerson* person_at_index;
     index=RNG_randrange(self->my_sim->rng,self->my_sim->procarray->pos)-1;
     person_at_index=(SirPerson*)Sim_get_process((Sim*)self->my_sim,index);
     while (self == person_at_index){
           index=RNG_randrange(self->my_sim->rng,self->my_sim->procarray->pos)-1;
           person_at_index=(SirPerson*)Sim_get_process((Sim*)self->my_sim,index);
     }
     return person_at_index;
}

void SirPerson_activate(SirPerson* self) {
     double ci;
     double ri;
     if(self->state==1){
           ci=SirPerson_contact_interval(self);
           SimProcess_set_event((SimProcess*)self,(FunctionPointer)SirPerson_live);
           Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
           self->is_activated=1;
           return;
     }
     if(self->state==2){
           ri=SirPerson_recovery_interval(self);
           SimProcess_set_event((SimProcess*)self,(FunctionPointer)SirPerson_live);
           Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ri,(SimProcess*)self);
           self->is_activated=1;
           return;
     }
}

void SirPerson_live(SirPerson* self) {
     double ci;
     double ri;
     double x;
     int infect;
     SirPerson* B;
     if(self->state==1){
            B=SirPerson_choose_contact(self);
            if(B->state==2){
                 if(self->my_sim->inf_prob==1.0){
                     infect=1;
                 }
                 else{
                     x=RNG_uniform(self->my_sim->rng);
                     if(x<self->my_sim->inf_prob){
                         infect=1;
                     }
                 }
                 if(infect==1){
                     self->my_sim->num_susceptible=self->my_sim->num_susceptible-1;
                     self->my_sim->num_infected=self->my_sim->num_infected+1;
                     fprintf(self->my_sim->dynamics_file,"%i\t%4.12f\t%i\t%i\t%i\n",self->my_sim->id,self->my_sim->current_time,self->my_sim->num_susceptible,self->my_sim->num_infected,self->my_sim->num_recovered);
                     fprintf(self->my_sim->phylogeny_file,"%i\t%4.12f\t%i\t%i\n",self->my_sim->id,self->my_sim->current_time,B->id,self->id);
                     self->state=2;
                     ri=SirPerson_recovery_interval(self);
                     Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ri,(SimProcess*)self);
                     return;
                     }
                 else{
                      ci=SirPerson_contact_interval(self);
                      Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
                     return;
                 }
            }
            else{
                 ci=SirPerson_contact_interval(self);
                 Sim_schedule((Sim*)self->my_sim,self->my_sim->current_time+ci,(SimProcess*)self);
                 return;
            }
     }
     if(self->state==2){
            self->my_sim->num_infected=self->my_sim->num_infected-1;
            self->my_sim->num_recovered=self->my_sim->num_recovered+1;
            fprintf(self->my_sim->dynamics_file,"%i\t%4.12f\t%i\t%i\t%i\n",self->my_sim->id,self->my_sim->current_time,self->my_sim->num_susceptible,self->my_sim->num_infected,self->my_sim->num_recovered);
            fprintf(self->my_sim->phylogeny_file,"%i\t%4.12f\t%i\t%i\n",self->my_sim->id,self->my_sim->current_time,self->id,-1);
            self->state=3;
            return;
     }
     
}

