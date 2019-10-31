#ifndef _SIM_H
#define _SIM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include "rng.h"
#include "eventlist.h"
#include "simprocess.h"
#include "array.h"

struct Sim {
    EventList* evlist;
    Array* procarray;
    RNG* rng;
    int id;
    double start_time;
    double stop_time;
    double current_time;
    int current_id;
    int number_of_processes;
    unsigned long seed;
    int run_flag;
    int pause_flag;
};

Sim* new_Sim(int id, double start, double stop, unsigned long seed, int capacity);
void delete_Sim(Sim* self);
void Sim_schedule(Sim* self, double t, SimProcess* p);
void Sim_append(Sim* self, SimProcess* p);
SimProcess* Sim_get_process(Sim* self, int t);
int Sim_get_id(Sim* self);
void Sim_do_all(Sim* self);
void Sim_do_until_pause(Sim* self);
void Sim_step_for_interval(Sim* self, double interval);
void Sim_step(Sim* self);

#ifdef __cplusplus
}
#endif
#endif

