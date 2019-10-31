CC   = gcc
OBJ  = siperson.o sisim.o sitest.o
LIBS =  -L../../epidemic -lepidemic -lm
INCS =  -I../../epidemic
BIN  = sitest.exe
CFLAGS = -ansi -pedantic -O2 $(INCS)  
RM = rm -f

all: sitest

clean:
	${RM} $(OBJ) $(BIN) $(all)

sitest: $(OBJ)
	$(CC) $(OBJ) -o sitest $(LIBS)

siperson.o: siperson.c
	$(CC) -c siperson.c -o siperson.o $(CFLAGS)

sisim.o: sisim.c
	$(CC) -c sisim.c -o sisim.o $(CFLAGS)

sitest.o: sitest.c
	$(CC) -c sitest.c -o sitest.o $(CFLAGS)

