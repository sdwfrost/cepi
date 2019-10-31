#include <assert.h>
#include "sirssim.h"
#include "sirsperson.h"

SirsSim* new_SirsSim(int id, double start_time, double stop_time, unsigned long seed, int capacity, char* dynamics_filename, char* phylogeny_filename) {
  SirsSim* self;
  self = (SirsSim*) malloc(sizeof(SirsSim));
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
  /* Set parameters  */
  self->inf_prob=0.0;
  self->contact_rate=0.0;
  self->recovery_rate=0.0;
  self->susceptibility_rate=0.0;
  /* Initial states */
  self->num_susceptible=0;
  self->num_infected=0;
  self->num_recovered=0;
  /* File names and handles */
  self->dynamics_filename=dynamics_filename;
  self->phylogeny_filename=phylogeny_filename;
  self->dynamics_file=fopen(self->dynamics_filename,"a");
  self->phylogeny_file=fopen(self->phylogeny_filename,"a");
  return self;
}

void delete_SirsSim(SirsSim* self) {
     int i;
     delete_RNG(self->rng);
     delete_EventList(self->evlist);
     for(i=0;i<(self->procarray->pos);i++){
          delete_SirsPerson(Array_at(self->procarray,i));
     }
     delete_Array(self->procarray);
     fclose(self->dynamics_file);
     fclose(self->phylogeny_file);
     free(self);
}

void SirsSim_initialize(SirsSim* self, double inf_prob, double contact_rate, double recovery_rate, double susceptibility_rate) {
     self->inf_prob=inf_prob;
     self->contact_rate=contact_rate;
     self->recovery_rate=recovery_rate;
     self->susceptibility_rate=susceptibility_rate;
}

void SirsSim_activate(SirsSim* self, int popsize, int num_infected, int num_recovered) {
     int index;
     SirsPerson* pers;
     for (index = 0; index < (popsize-num_infected-num_recovered); ++index) {
         pers=new_SirsPerson(self,1);
         SirsPerson_activate(pers);
         Array_append(self->procarray,(void*)pers);
         self->num_susceptible=self->num_susceptible+1;
     }
     for (index = (popsize-num_infected-num_recovered); index < (popsize-num_recovered); ++index) {
         pers=new_SirsPerson(self,2);
         SirsPerson_activate(pers);
         Array_append(self->procarray,(void*)pers);
         self->num_infected=self->num_infected+1;
         fprintf(self->phylogeny_file,"%i\t%4.12f\t%i\t%i\n",self->id,self->current_time,-1,pers->id);
     }
     for (index = (popsize-num_recovered); index < popsize; ++index) {
         pers=new_SirsPerson(self,3);
         SirsPerson_activate(pers);
         Array_append(self->procarray,(void*)pers);
         self->num_recovered=self->num_recovered+1;
         fprintf(self->phylogeny_file,"%i\t%4.12f\t%i\t%i\n",self->id,self->current_time,-1,pers->id);
     }
     fprintf(self->dynamics_file,"%i\t%4.12f\t%i\t%i\t%i\n",self->id,self->current_time,self->num_susceptible,self->num_infected,self->num_recovered);
}     

