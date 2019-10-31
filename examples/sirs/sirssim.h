#ifndef _SIRSSIM_H
#define _SIRSSIM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "sim.h"

typedef struct SirsSim {
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
    double recovery_rate;
    double susceptibility_rate;
    int num_susceptible;
    int num_infected;
    int num_recovered;
    char* dynamics_filename;
    char* phylogeny_filename;
    FILE* dynamics_file;
    FILE* phylogeny_file;
} SirsSim;
SirsSim* new_SirsSim(int id, double start, double stop, unsigned long seed, int capacity, char* dynamics_filename, char* phylogeny_filename);
void delete_SirsSim(SirsSim* self);
void SirsSim_initialize(SirsSim* self, double inf_prob, double contact_rate, double recovery_rate, double susceptibility_rate);
void SirsSim_activate(SirsSim* self, int popsize, int num_infected, int num_recovered);

#ifdef __cplusplus
}
#endif
#endif
