#ifndef _SIRSPERSON_H
#define _SIRSPERSON_H
#ifdef __cplusplus
extern "C" {
#endif
#include "simprocess.h"
#include "sirssim.h"

typedef struct SirsPerson {
  FunctionPointer next_event_func;
  void* next_event_data;
  int id;
  int is_activated;
  SirsSim* my_sim;
  int state;
 } SirsPerson;

SirsPerson* new_SirsPerson(SirsSim* my_sim, int state);
void delete_SirsPerson(SirsPerson* self);
double SirsPerson_contact_interval(SirsPerson* self);
double SirsPerson_recovery_interval(SirsPerson* self);
double SirsPerson_susceptibility_interval(SirsPerson* self);
SirsPerson* SirsPerson_choose_contact(SirsPerson* self);
void SirsPerson_activate(SirsPerson* self);
void SirsPerson_live(SirsPerson* self);

#ifdef __cplusplus
}
#endif
#endif
