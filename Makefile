CC=gcc
CFLAGS= -std=c99 -O3 -fPIC -Wall  

SOURCES=perturbations.c kepler_solve.c integration_steps.c initialization.c resonant_interactions.c LaplaceCoefficients.c
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)
	$(CC) $(CFLAGS) -lm  -shared -o libSemiNbody.so $^
%.o: %.c
	$(CC)  -c -o $@ $< $(CFLAGS)
testme: testme.c $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -rf *.o *.so
run: testme
	./testme > data.txt
