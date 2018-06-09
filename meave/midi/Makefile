SHELL := /bin/bash

ROOT := $(abspath $(shell x='.' && while true; do [ -e "$$x/meave" ] && echo "$$x" && break; x="$$x/.."; echo >&2 "$$x"; done))
CC := g++

PKG_CONFIG_PATH = $(ROOT)/_/_glog/lib/pkgconfig

MEAVE_CPPFLAGS = -std=gnu++17 -I$(ROOT)

GLOG_CPPFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags libglog)
GLOG_LDFLAGS = $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs libglog)

BOOST_CPPFLAGS = -I$(ROOT)/_/_boost/include
BOOST_LDFLAGS = -L$(ROOT)/_/_boost/lib -lboost_system

CPPFLAGS += ${MEAVE_CPPFLAGS} ${GLOG_CPPFLAGS} ${BOOST_CPPFLAGS} -Wall -Werror -O0 -ggdb
LDFLAGS += -Wl,-rpath=${ROOT}/_/_glog/lib ${GLOG_LDFLAGS} ${BOOST_CPPFLAGS} -lpthread

.PHONY: all
all: fileview_main parser_main

# TODO: Rules don't reflect dependencies on header files!
parser_main.o: parser_main.cpp parser.hpp
	${CC} ${CPPFLAGS} -o parser_main.o -c parser_main.cpp

parser_main: parser_main.o
	${CC} parser_main.o ${LDFLAGS} -o parser_main

fileview_main.o: fileview_main.cpp fileview.hpp
	${CC} ${CPPFLAGS} -o fileview_main.o -c fileview_main.cpp

fileview_main: fileview_main.o
	${CC} fileview_main.o ${LDFLAGS} -o fileview_main

clean:
	rm -vf *.o fileview_main
