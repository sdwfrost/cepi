#ifndef _EVENTLIST_H
#define _EVENTLIST_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>
#include "avl.h"
#include "simprocess.h"
#include "event.h"

typedef struct EventList {
    struct avl_table* tree;
    struct avl_traverser trav;
    double t;
    SimProcess* p;
    int count;
} EventList;

EventList* new_EventList(void);
void delete_EventList(EventList* self);
void EventList_insert(EventList* self, double t, SimProcess* p);
void EventList_next(EventList* self);
void EventList_print_timestamps(EventList* self);

#ifdef __cplusplus
}
#endif
#endif

