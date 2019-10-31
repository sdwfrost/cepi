CC   = gcc
OBJ  = sirperson.o sirsim.o sirtest.o
LIBS =  -L../../epidemic -lepidemic -lm
INCS =  -I../../epidemic
BIN  = sirtest
CFLAGS = -ansi -pedantic -O2 $(INCS)  
RM = rm -f

all: sirtest

clean:
	${RM} $(OBJ) $(BIN) $(all)

sirtest: $(OBJ)
	$(CC) $(OBJ) -o sirtest $(LIBS)

sirperson.o: sirperson.c
	$(CC) -c sirperson.c -o sirperson.o $(CFLAGS)

sirsim.o: sirsim.c
	$(CC) -c sirsim.c -o sirsim.o $(CFLAGS)

sirtest.o: sirtest.c
	$(CC) -c sirtest.c -o sirtest.o $(CFLAGS)

