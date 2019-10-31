#ifndef _SIMPROCESS_H
#define _SIMPROCESS_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef void (*FunctionPointer)(void*);

typedef struct Sim Sim;

int Sim_get_id(Sim* self);

typedef struct SimProcess {
    FunctionPointer next_event_func;
    void* next_event_data;
    int id;
    int is_activated;
    Sim* my_sim;
} SimProcess;

SimProcess* new_SimProcess(Sim* my_sim);
void delete_SimProcess(SimProcess* self);
void SimProcess_set_event(SimProcess* self, FunctionPointer func);
void SimProcess_set_event_with_data(SimProcess* self, FunctionPointer func, void* data);

#ifdef __cplusplus
}
#endif
#endif
