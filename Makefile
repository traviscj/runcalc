
CPP=g++
CFLAGS=-g
LDFLAGS= 

all: runcalc

runcalc: runcalc.cpp
	$(CPP) -o $@ $(CFLAGS) $(LDFLAGS) $<

clean:
	rm runcalc

rebuild: clean all

.PHONY: clean rebuild all
