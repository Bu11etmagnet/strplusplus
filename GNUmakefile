CXX ?= g++
CC := ${CXX}

CXXFLAGS := -pthread -Wall -Wpedantic -Wextra -Werror -g -std=c++17
LDFLAGS  := -pthread
LDLIBS   := -lgtest


run: strpp_test
	./$<

all: strpp_test


strpp_test: strpp_test.o strpp.o
	${LINK.o} $^ ${LOADLIBES} ${LDLIBS} -o $@

