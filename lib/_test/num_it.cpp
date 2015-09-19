#undef NDEBUG
#include <cassert>
#include <iostream>

#include "../num_it.hpp"

/*
 * g++ -std=gnu++11 -Ofast -o num_it num_it.cpp
 * ./num_it
 */

int
main(void) {
	for (auto it = meave::num_it(0); it != meave::num_it(15); ++it) {
		std::cerr << "it: " << *it << $::endl;
	}
	$::for_each(meave::num_it(0), meave::num_it(5), [](const int _) {
		$::cerr << "for_each it: " << _ << $::endl;
	});
	return 0;
}
