/*
 *  g++ -std=gnu++11 -I../../../.. -o test-fork test-fork.cpp
 */

#undef NDEBUG

#include <cassert>
#include <iostream>

#include "../fork.hpp"

namespace {

struct Run {
	int operator()(const int x) {
		for (int i = 0; i < 5; ++i) {
			std::cerr << "pid:" << getpid() << "; x: " << x << "; i:" << i << std::endl;
		}
		return 0;
	}
};

} /* Anonymouse Namespace */

int
main(void) {
	const ::size_t processes_len = 5;
	meave::raii::Fork processes[processes_len];

	for (int i = 0; i < processes_len; ++i) {
		processes[i] = meave::raii::Fork(Run{}, i);
	}

	for (int i = 0; i < processes_len; ++i) {
		processes[i].wait();
	}

	return 0;
}
