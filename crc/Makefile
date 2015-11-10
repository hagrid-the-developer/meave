CC ?= g++
CXXFLAGS += -std=gnu++1y -I..

COMP.cpp = $(CC) $(CXXFLAGS) -o $@ -c $<

all: compare_speed

compare_speed: compare_speed.o crc32_intel_asm.o
	g++ -o $@ $^

%.o: %.S
	as $<

compare_speed.o: compare_speed.cpp funcs.h
	$(COMP.cpp)
