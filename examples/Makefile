all: sitest sirtest sirstest treesampler

sitest:
	cd si; $(MAKE) all

sirtest:
	cd sir; $(MAKE) all

sirstest:
	cd sirs; $(MAKE) all

treesampler:
	cd treesampler; $(MAKE) all

clean:
	cd si; rm -f *.o sitest
	cd sir; rm -f *.o sirtest
	cd sirs; rm -f *.o sirstest
	cd treesampler; rm -f *.o treesampler treesampler_nex

