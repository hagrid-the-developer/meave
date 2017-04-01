CC = gcc
CXX = g++
AS = as
CFLAGS += -std=gnu11 -I../../../.. -Ofast  -march=core-avx-i -mtune=core-avx-i -mavx -mavx2

CXXFLAGS += -std=gnu++1y
ASFLAGS += -march=avx2

COMP.c = $(CC) $(CFLAGS) -o $@ -c $<
COMP.cpp = $(CXX) $(CFLAGS:-std%=) -std=gnu++1y $(CXXFLAGS) -o $@ -c $<
COMP.S = $(AS) -o $@ $<
LINK.o = $(CXX) -o $@ $^

all: run.test-rolhash

clean:
	rm -vf *.o test-rolhash ../*.o

.PHONY: run.test-rolhash
run.test-rolhash: test-rolhash
	for x in `seq 16 16 20480`; do ./test-rolhash "$$x"; done 2>test-rolhash.res
	head test-rolhash.res

test-rolhash: test-rolhash.o ../rolhash_kernels.o
	$(LINK.o)

%.o: %.cpp
	$(COMP.cpp)

%.o: %.c
	$(COMP.c)

%.o: %.S
	$(COMP.S)
