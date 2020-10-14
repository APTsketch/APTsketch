GCC = g++
CFLAGS = -O2 -w -std=c++11 
SSEFLAGS =   -march=native
FILES = sim.out

all: $(FILES)

sim.out: GenData.cpp
	$(GCC) $(CFLAGS)  -o sim.out GenData.cpp
.PHONY: clean
clean:
	-rm -f *.out $(objs)
