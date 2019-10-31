#ifndef _EVENT_H
#define _EVENT_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>

typedef struct Event {
    double t;
    void* item;
} Event;

Event* new_Event(double t, void* item);
void delete_Event(Event* self);
int compare_times (const void *event_a, const void *event_b, void *avl_param);

#ifdef __cplusplus
}
#endif
#endif
