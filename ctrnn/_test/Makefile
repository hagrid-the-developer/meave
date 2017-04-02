SHELL := /bin/bash

ROOT := $(shell x='.' && while true; do [ -e "$$x/_" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done)
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig:$(ROOT)/_/_jansson/lib/pkgconfig

BOOST_CPPFLAGS = -I$(ROOT)/_/_boost/include
# We use static linking to avoid issues with finding of boost and glog libs
BOOST_LDFLAGS = -static -L$(ROOT)/_/_boost/lib -lboost_system

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

JASSON_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags jansson)
JASSON_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs jansson)

MEAVE_CPPFLAGS = -std=gnu++1y -I$(ROOT)/..

CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} ${JASSON_CPPFLAGS} -fextended-identifiers
LDFLAGS += ${BOOST_LDFLAGS} ${GLOG_LDFLAGS} ${JASSON_LDFLAGS} -lpthread

all: test.neuron-state

neuron-state.o: neuron-state.cpp
	$(ROOT)/utils/extended-chars.pl <neuron-state.cpp >neuron-state.xyz.cpp
	${CC} ${CPPFLAGS} -o neuron-state.o -c neuron-state.xyz.cpp

neuron-state: neuron-state.o
	${CC} neuron-state.o ${LDFLAGS} -o neuron-state

test.neuron-state: neuron-state
	./neuron-state

clean: rm -v *.o ./neuron-state
