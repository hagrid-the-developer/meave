CC = gcc
CXX = g++
AS = as
CFLAGS += -std=gnu11 -I../../../.. -Ofast -march=native -mavx -mavx2

CXXFLAGS += -std=gnu++1y
ASFLAGS += -march=avx2


COMP.c = $(CC) $(CFLAGS) -o $@ -c $<
COMP.cpp = $(CXX) $(CFLAGS:-std%=) $(CXXFLAGS) -o $@ -c $<
COMP.S = $(AS) -o $@ $< 
LINK.o = $(CXX) -o $@ $^	

all: run.test-exp-pade22 run.test-exp2-taylor4

clean:
	rm -vf *.o test-exp-pade22 test-exp2-taylor4

.PHONY: run.test-exp-pade22
run.test-exp-pade22: test-exp-pade22
	./test-exp-pade22

test-exp-pade22: test-exp-pade22.o ../exp_pade22.o
	$(LINK.o)

.PHONY: run.test-exp2-taylor4
run.test-exp2-taylor4: test-exp2-taylor4
	./test-exp2-taylor4

test-exp2-taylor4: test-exp2-taylor4.o ../exp2_taylor.o ../avx2_math.o
	$(LINK.o)

%.o: %.cpp
	$(COMP.cpp)

%.o: %.c
	$(COMP.c)

%.o: %.S
	$(COMP.S)
