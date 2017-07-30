CC ?= gcc
CXX ?= g++
AS ?= as
CFLAGS += -std=gnu11 -I../../../.. -I../../../../_/_glog/include -Ofast -march=native -mavx -mavx2

CXXFLAGS += -std=gnu++17
ASFLAGS += -march=avx2


COMP.c = $(CC) $(CFLAGS) -o $@ -c $<
COMP.cpp = $(CXX) $(CFLAGS:-std%=) $(CXXFLAGS) -o $@ -c $<
COMP.S = $(AS) -o $@ $< 
LINK.o = $(CXX) -o $@ $^	

.PHONY: all
all: run.test_optmnd

.PHONE: clean
clean:
	rm -vf *.o test_optmnd

.PHONY: run.test_optmnd
run.test_optmnd: test_optmnd
	./test_optmnd

test_optmnd: test_optmnd.o
	$(LINK.o)

# TODO: Dependencies upon header files!
%.o: %.cpp
	$(COMP.cpp)

%.o: %.c
	$(COMP.c)

%.o: %.S
	$(COMP.S)
