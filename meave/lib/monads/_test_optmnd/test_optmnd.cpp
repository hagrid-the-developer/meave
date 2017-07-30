#include <iostream>

#include <meave/lib/monads/optmnd.hpp>

namespace {

meave::monad::OptMnd<int> x(int $) {
	return 5 + $;
}

meave::monad::OptMnd<int> y(int $) {
	return 7*$;
}

meave::monad::OptMnd<float> f(int $) {
	return $/10.f;
}

} /* Anonymous Namespace */

int
main(void) {
	auto val = x(11) >> y >> f;

	$::cerr << "val: " << *val << $::endl;

	return 0;
}
