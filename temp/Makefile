SOURCEDIR=src/
OUTDIR=bin/
CXX=icpc
INCLUDES= -L/share/apps/papi/5.4.1/lib -I/share/apps/papi/5.4.1/include -lpapi
CXXFLAGS= -O2 -no-vec -std=c++11 -mavx -g -qopenmp -qopt-report5 $(INCLUDES)

PROGS=$(OUTDIR)matMult

.PHONY: default

default: $(OUTDIR) $(SOURCEDIR)matMult.c
	$(CXX) $(CXXFLAGS) -o $(OUTDIR)matMult $(SOURCEDIR)matMult.c $(LIBS)

$(OUTDIR):
	mkdir $(OUTDIR)

.PHONY: clean
clean:
	rm -r $(OUTDIR)
