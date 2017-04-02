SHELL := /bin/bash

ROOT := $(shell x='.' && while true; do [ -e "$$x/_" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done)
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig:$(ROOT)/_/_jansson/lib/pkgconfig

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

MEAVE_CPPFLAGS = -std=gnu++1z -I$(ROOT)/.. -fopenmp
MEAVE_LDFLAGS = -fopenmp

#CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} ${JASSON_CPPFLAGS} -Wall -Werror -O0 -pthread -ggdb -finput-charset=UTF-8
CPPFLAGS += ${MEAVE_CPPFLAGS} ${GLOG_CPPFLAGS} -Wall -Werror -Ofast -ftree-vectorize -ffast-math -omit-frame-pointer -pthread -finput-charset=UTF-8 -DNDEBUG -fdiagnostics-color=auto
LDFLAGS += ${MEAVE_LDFLAGS} ${GLOG_LDFLAGS} -pthread

all: simple-trial

simple-trial.o: simple-trial.cpp simple-trial.hpp
	${CC} ${CPPFLAGS} -o simple-trial.o -c simple-trial.cpp

simple-trial: simple-trial.o
	${CC} simple-trial.o ${LDFLAGS} -o simple-trial

clean:
	rm -vf *.o ./simple-trial
