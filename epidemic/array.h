#ifndef _ARRAY_H
#define _ARRAY_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>

typedef struct Array {
    int pos;
    int capacity;
    void** items;
} Array;

Array* new_Array(int capacity);
void Array_append(Array* self, void* item);
void* Array_at(Array* self, int pos);
void delete_Array(Array* self);

#ifdef __cplusplus
}
#endif
#endif

