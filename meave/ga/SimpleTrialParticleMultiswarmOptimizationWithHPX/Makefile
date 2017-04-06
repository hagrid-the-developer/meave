SHELL := /bin/bash

ROOT := $(realpath $(shell x='.' && while true; do [ -e "$$x/_" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done))
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig:$(ROOT)/_/_jansson/lib/pkgconfig:$(ROOT)/_/_hpx/lib/pkgconfig

BOOST_CPPFLAGS = -I$(ROOT)/_/_boost/include
BOOST_LDFLAGS = -L$(ROOT)/_/_boost/lib -lboost_system

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

JASSON_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags jansson)
JASSON_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs jansson)

HPX_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags hpx_application hpx_component)
HPX_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs hpx_application hpx_component)

MEAVE_CPPFLAGS = -std=gnu++1y -I$(ROOT)/..

CPPFLAGS += ${MEAVE_CPPFLAGS} ${BOOST_CPPFLAGS} ${GLOG_CPPFLAGS} ${JASSON_CPPFLAGS} ${HPX_CPPFLAGS} -fextended-identifiers -Wall -Werror -O0 -pthread -g -fexec-charset=UTF-8 -finput-charset=UTF-8
LDFLAGS += ${BOOST_LDFLAGS} ${GLOG_LDFLAGS} ${JASSON_LDFLAGS} ${HPX_LDFLAGS} -lpthread
COMMA := ,
RPATH += $(patsubst -L%, -Wl$(COMMA)-rpath=%, $(filter -L%,$(LDFLAGS)))

all: simple-trial

simple-trial.o: simple-trial.cpp simple-trial.hpp
	${CC} ${CPPFLAGS} -o simple-trial.o -c simple-trial.cpp

simple-trial: simple-trial.o
	${CC} simple-trial.o ${LDFLAGS} ${RPATH} -o simple-trial

clean:
	rm -vf *.o ./simple-trial
