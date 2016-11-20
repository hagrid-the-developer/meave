#include <iostream>

#include "config.hpp"

int
main(int argc, char *argv[])
{
	namespace me = meave::examples;

	me::Config config;
       	if (!config.parse(argc, argv))
		return 1;

	$::cout << config << $::endl;

}
