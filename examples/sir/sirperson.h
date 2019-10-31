#ifndef _SIRPERSON_H
#define _SIRPERSON_H
#ifdef __cplusplus
extern "C" {
#endif
#include "simprocess.h"
#include "sirsim.h"

typedef struct SirPerson {
  FunctionPointer next_event_func;
  void* next_event_data;
  int id;
  int is_activated;
  SirSim* my_sim;
  int state;
 } SirPerson;

SirPerson* new_SirPerson(SirSim* my_sim, int state);
void delete_SirPerson(SirPerson* self);
double SirPerson_contact_interval(SirPerson* self);
double SirPerson_recovery_interval(SirPerson* self);
SirPerson* SirPerson_choose_contact(SirPerson* self);
void SirPerson_activate(SirPerson* self);
void SirPerson_live(SirPerson* self);

#ifdef __cplusplus
}
#endif
#endif
