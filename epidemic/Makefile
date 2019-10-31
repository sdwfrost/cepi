CC   = gcc
AR = ar
OBJ  = array.o avl.o event.o eventlist.o simprocess.o rng.o mt19937p.o sim.o
LIBS =  -lm
INCS =  
BIN  = libepidemic.a
CFLAGS = -ansi -pedantic -O2 $(INCS)

all: libepidemic

libepidemic: $(OBJ)
	$(AR) rcs libepidemic.a $(OBJ)

array.o: array.c
	$(CC) -c array.c -o array.o $(CFLAGS)

avl.o: avl.c
	$(CC) -c avl.c -o avl.o $(CFLAGS)

event.o: event.c
	$(CC) -c event.c -o event.o $(CFLAGS)

eventlist.o: eventlist.c
	$(CC) -c eventlist.c -o eventlist.o $(CFLAGS)

simprocess.o: simprocess.c
	$(CC) -c simprocess.c -o simprocess.o $(CFLAGS)

rng.o: rng.c
	$(CC) -c rng.c -o rng.o $(CFLAGS)

mt19937p.o: mt19937p.c
	$(CC) -c mt19937p.c -o mt19937p.o $(CFLAGS)

rngtest.o: rngtest.c
	$(CC) -c rngtest.c -o rngtest.o $(CFLAGS)

sim.o: sim.c
	$(CC) -c sim.c -o sim.o $(CFLAGS)

