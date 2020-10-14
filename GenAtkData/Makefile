GCC = g++
CFLAGS = -O2 -w -std=c++11 
SSEFLAGS =   -march=native
FILES = sim.out

all: $(FILES)

sim.out: GenAtkData.cpp
	$(GCC) $(CFLAGS)  -o sim.out GenAtkData.cpp
.PHONY: clean
clean:
	-rm -f *.out $(objs)
