CC ?= gcc
CXX ?= g++
CFLAGS += -std=gnu11 -I.. -Ofast -msse4.2 -mcrc32
CXXFLAGS += -std=gnu++1y -I.. -Ofast


COMP.c = $(CC) $(CFLAGS) -o $@ -c $<
COMP.cpp = $(CXX) $(CXXFLAGS) -o $@ -c $<

all: compare_speed

clean:
	rm -v *.o compare_speed

compare_speed: compare_speed.o crc_test.o crc32_intel_asm.o sse42_crc32.o
	g++ -o $@ $^

%.o: %.S
	as $< -o $@

sse42_crc32.o: sse42_crc32.c funcs.h
	$(COMP.c)
crc_test.o: crc_test.cpp crc_test.hpp
	$(COMP.cpp)
compare_speed.o: compare_speed.cpp funcs.h
	$(COMP.cpp)
