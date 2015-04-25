/*
 * g++ -std=gnu++11 -o test-error test-error.cpp
 */

#undef NDEBUG
#include <cassert>
#include <iostream>

#include "../error.hpp"

int
main(void) {
	try {
		try {
			errno = EAGAIN;
			throw meave::Error("Hello World: %m");
		} catch (meave::Error e) {
			throw meave::Error("Error occured: ") << e;
		}
	} catch (meave::Error e) {
		std::cerr << "e:" << e.what() << ";" << std::endl;
	}

	try {
		throw meave::Error() << "Hello" << " : " << "World";
	} catch (meave::Error e) {
		std::cerr << "e:" << e.what() << ";" << std::endl;
	}

	return 0;
}
