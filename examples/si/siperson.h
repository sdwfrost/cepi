#ifndef _SIPERSON_H
#define _SIPERSON_H
#ifdef __cplusplus
extern "C" {
#endif
#include "simprocess.h"
#include "sisim.h"

typedef struct SiPerson {
  FunctionPointer next_event_func;
  void* next_event_data;
  int id;
  int is_activated;
  SiSim* my_sim;
  int state;
 } SiPerson;

SiPerson* new_SiPerson(SiSim* my_sim, int state);
void delete_SiPerson(SiPerson* self);
double SiPerson_contact_interval(SiPerson* self);
SiPerson* SiPerson_choose_contact(SiPerson* self);
void SiPerson_activate(SiPerson* self);
void SiPerson_live(SiPerson* self);

#ifdef __cplusplus
}
#endif
#endif
