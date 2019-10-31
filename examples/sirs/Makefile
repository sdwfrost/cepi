CC   = gcc
OBJ  = sirsperson.o sirssim.o sirstest.o
LIBS =  -L../../epidemic -lepidemic -lm
INCS =  -I../../epidemic
BIN  = sisrtest
CFLAGS = -ansi -pedantic -O2 $(INCS)  
RM = rm -f

all: sirstest

clean:
	${RM} $(OBJ) $(BIN) $(all)

sirstest: $(OBJ)
	$(CC) $(OBJ) -o sirstest $(LIBS)

sirsperson.o: sirsperson.c
	$(CC) -c sirsperson.c -o sirsperson.o $(CFLAGS)

sirssim.o: sirssim.c
	$(CC) -c sirssim.c -o sirssim.o $(CFLAGS)

sirstest.o: sirstest.c
	$(CC) -c sirstest.c -o sirstest.o $(CFLAGS)

