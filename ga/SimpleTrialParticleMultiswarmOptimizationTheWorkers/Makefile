SHELL := /bin/bash

ROOT := $(shell x='.' && while true; do [ -e "$$x/_" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done)
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig:$(ROOT)/_/_jansson/lib/pkgconfig

BOOST_CPPFLAGS = -I$(ROOT)/_/_boost/include
# We use static linking to avoid issues with finding of boost and glog libs
BOOST_LDFLAGS = -L$(ROOT)/_/_boost/lib -lboost_system

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

JASSON_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags jansson)
JASSON_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs jansson)

MEAVE_CPPFLAGS = -std=gnu++1z -I$(ROOT)/..

#CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} ${JASSON_CPPFLAGS} -Wall -Werror -O0 -pthread -g -finput-charset=UTF-8
CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} ${JASSON_CPPFLAGS} -Wall -Werror -Ofast -ftree-vectorize -pthread -finput-charset=UTF-8 -DNDEBUG -fdiagnostics-color=auto
LDFLAGS += ${BOOST_LDFLAGS} ${GLOG_LDFLAGS} ${JASSON_LDFLAGS} -pthread

all: simple-trial

simple-trial.o: simple-trial.cpp simple-trial.hpp
	${CC} ${CPPFLAGS} -o simple-trial.o -c simple-trial.cpp

simple-trial: simple-trial.o
	${CC} simple-trial.o ${LDFLAGS} -o simple-trial

clean:
	rm -vf *.o ./simple-trial
