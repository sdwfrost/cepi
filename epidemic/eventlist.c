#include "eventlist.h"
#include <stdio.h>
#include <float.h>

EventList* new_EventList(void) {
  EventList* self;
  self = (EventList*) malloc(sizeof(EventList));
  assert(self!=NULL);
  self->tree=avl_create(compare_times, NULL, NULL);
  avl_t_init(&self->trav,self->tree);
  self->t=0.0;
  self->p=NULL;
  self->count=0;
  return self;
}

void delete_EventList(EventList* self) {
  avl_destroy(self->tree,NULL);
  free(self);
}

void EventList_insert(EventList* self, double t, SimProcess* p) {
  Event* ev = new_Event(t,(void*)p);
  assert(ev!=NULL);
  avl_insert(self->tree, ev);
  self->count=self->count+1;
}

void EventList_next(EventList* self) {
	Event* next_event;
	next_event=(Event*)avl_t_first(&self->trav,self->tree);
	assert(next_event!=NULL);
	avl_delete(self->tree,next_event); 
	self->count=self->count-1;
	if ((self->count)>=0)
	{
		self->t=next_event->t;
		self->p=(SimProcess*)next_event->item; 
	}
	else
	{
		self->t = FLT_MAX;
		self->p = NULL;
	}
	delete_Event(next_event);
}   

void EventList_print_timestamps(EventList* self) {
	int i;
	Event* next_event;
    next_event=(Event*)avl_t_first(&self->trav,self->tree);
	for(i=0;i<self->count;++i){
		printf("%f\t%i\n",next_event->t,((SimProcess*)(next_event->item))->id);
		next_event=(Event*)avl_t_next(&self->trav);
	}
	delete_Event(next_event);
}
