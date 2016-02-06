SHELL := /bin/bash

CXX = g++
CXXFLAGS += -std=gnu++1y -I../../../..

LDFLAGS += -lboost_program_options -lboost_system

COMPILE.cpp = $(CXX) $(CXXFLAGS) -o $@ -c $<
LINK.o = $(CXX) -o $@ $^ $(LDFLAGS)

all: main

config.o: config.cpp config.hpp
	$(COMPILE.cpp)

main.o: main.cpp
	$(COMPILE.cpp)

main: LDFLAGS += -lboost_system
main: main.o config.o
	$(LINK.o)
	
