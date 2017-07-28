SHELL := /bin/bash

ifndef MKLROOT
$(error Please run . /path/to/mkl/bin/compilervars.sh first!)
endif

ROOT := $(shell x='.' && while true; do [ -e "$$x/_" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done)
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig

BOOST_CPPFLAGS = -I$(ROOT)/_/_boost/include
# We use static linking to avoid issues with finding of boost and glog libs
BOOST_LDFLAGS = -static -L$(ROOT)/_/_boost/lib -lboost_system

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

MEAVE_CPPFLAGS = -std=gnu++1y -I$(ROOT) -mavx2 -O0 -ggdb

CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} -fextended-identifiers
LDFLAGS += ${BOOST_LDFLAGS} ${GLOG_LDFLAGS} -lpthread

.PHONY: all
all: test.neuron-state-mkl

neuron-state.o: neuron-state.cpp
	# https://gcc.gnu.org/wiki/FAQ#utf8_identifiers
	$(ROOT)/utils/extended-chars.pl <neuron-state.cpp >neuron-state.xyz.cpp
	${CC} ${CPPFLAGS} -o neuron-state.o -c neuron-state.xyz.cpp

neuron-state: neuron-state.o
	${CC} neuron-state.o ${LDFLAGS} -o neuron-state

neuron-state-mkl.o: CPPFLAGS += -I${MKLROOT}/include
neuron-state-mkl.o: neuron-state-mkl.cpp
	# https://gcc.gnu.org/wiki/FAQ#utf8_identifiers
	$(ROOT)/utils/extended-chars.pl <neuron-state-mkl.cpp >neuron-state-mkl.xyz.cpp
	${CC} ${CPPFLAGS} -o neuron-state-mkl.o -c neuron-state-mkl.xyz.cpp

neuron-state-mkl: LDFLAGS += -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a ${MKLROOT}/lib/intel64/libmkl_gnu_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -lgomp -lpthread -lm -ldl
neuron-state-mkl: neuron-state-mkl.o
	${CC} neuron-state-mkl.o ${LDFLAGS} -o neuron-state-mkl

test.neuron-state-mkl: neuron-state neuron-state-mkl
	./neuron-state-mkl

.PHONY: clean
clean:
	rm -fv *.o ./neuron-state ./neuron-state-mkl
