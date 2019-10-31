all: 
	cd epidemic; $(MAKE) all
	cd examples; $(MAKE) all
clean:
	cd epidemic; rm -f *.a *.o
	cd examples; $(MAKE) clean

