#undef NDEBUG
#include <cassert>
#include <iostream>
#include <vector>

#include "../conv_it.hpp"

/*
 * $ g++ -std=gnu++11 -Ofast -o conv_it conv_it.cpp
 * $ ./conv_it
 */

int
main(void) {
	int arr[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
	::size_t arr_len = sizeof(arr) / sizeof(*arr);

	auto f = [](const int _) -> int { return 2*_; };

	std::vector<int> vec(&arr[0], &arr[arr_len]);
	for (auto it = meave::conv_it(vec.begin(), f); it != vec.end(); ++it) {
		$::cerr << "it: " << *it << $::endl;
	}

	std::cerr << "----" << std::endl;

	auto g = [](const int _) -> float { return _ / 10.f; };

	for (auto it = meave::conv_it(vec.begin(), g); it != vec.end(); ++it) {
		$::cerr << "it: " << *it << $::endl;
	}

	return 0;
}
