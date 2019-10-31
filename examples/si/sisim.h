#ifndef _SISIM_H
#define _SISIM_H
#ifdef __cplusplus
extern "C" {
#endif
#include "sim.h"

typedef struct SiSim {
    /* From Sim */
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
    /* Specific to this model */
    double inf_prob;
    double contact_rate;
    int num_infected;
} SiSim;
SiSim* new_SiSim(int id, double start, double stop, unsigned long seed, int capacity);
void delete_SiSim(SiSim* self);
void SiSim_initialize(SiSim* self, double inf_prob, double contact_rate);
void SiSim_activate(SiSim* self, int popsize, int num_infected);

#ifdef __cplusplus
}
#endif
#endif
