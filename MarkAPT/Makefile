GCC = g++
CFLAGS = -O2 -w -std=c++11
SSEFLAGS =   -march=native
FILES = mark.out

all: $(FILES)

mark.out: MarkAPT.cpp
	$(GCC) $(CFLAGS)  -o mark.out MarkAPT.cpp
.PHONY: clean
clean:
	-rm -f *.out $(objs)
