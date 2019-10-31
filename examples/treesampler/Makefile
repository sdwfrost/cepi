CC   = gcc
OBJ  = avl.o rand.o
BIN = treesampler treesampler_nex
LIBS =  -lm
INCS =  -I .
CFLAGS = -std=c99 -pedantic -O2 $(INCS)  
RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN) treesampler.o treesampler_nex.o

treesampler: $(OBJ) treesampler.o
	$(CC) $(OBJ) treesampler.o -o treesampler $(LIBS)

treesampler_nex: $(OBJ) treesampler_nex.o
	$(CC) $(OBJ) treesampler_nex.o -o treesampler_nex $(LIBS)

avl.o: avl.c
	$(CC) -c avl.c -o avl.o $(CFLAGS)

rand.o: rand.c
	$(CC) -c rand.c -o rand.o $(CFLAGS)

treesampler.o: treesampler.c
	$(CC) -c treesampler.c -o treesampler.o $(CFLAGS)

treesampler_nex.o: treesampler_nex.c
	$(CC) -c treesampler_nex.c -o treesampler_nex.o $(CFLAGS)

