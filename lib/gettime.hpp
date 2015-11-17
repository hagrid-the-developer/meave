#ifndef MEAVE_GETTIME_HPP_INCLUDED
#define MEAVE_GETTIME_HPP_INCLUDED

#include <cstdlib>
#include <sys/time.h>
#include <time.h>

namespace meave {

namespace {

double gettime() noexcept {
	struct timeval tv;
	if (-1 == gettimeofday(&tv, nullptr))
		::abort(); // This could only happen by some mistake in the program.
	return tv.tv_sec + tv.tv_usec/1000000.0;
}

double getrealtime() noexcept {
	struct timespec ts;
	if (-1 == clock_gettime(CLOCK_REALTIME, &ts))
		::abort(); // This could only happen by some mistake in the program.
	return ts.tv_sec + ts.tv_nsec/1000000000.0;
}

} /* anonymous namespace */

} /* namespace meave */

#endif // MEAVE_GETTIME_HPP_INCLUDED
