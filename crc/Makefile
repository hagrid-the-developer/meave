CC ?= g++
CXXFLAGS += -std=gnu++1y -I.. -Ofast

COMP.cpp = $(CC) $(CXXFLAGS) -o $@ -c $<

all: compare_speed

compare_speed: compare_speed.o crc_test.o crc32_intel_asm.o
	g++ -o $@ $^

%.o: %.S
	as $< -o $@

crc_test.o: crc_test.cpp crc_test.hpp
	$(COMP.cpp)
compare_speed.o: compare_speed.cpp funcs.h
	$(COMP.cpp)
