#ifndef MEAVE_SEED_HPP
#	define MEAVE_SEED_HPP

#include "meave/commons.hpp"

#include <cstdlib>
#include <functional>
#include <thread>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace meave {

unsigned seed() noexcept {
	struct timeval tv;
	if (-1 == gettimeofday(&tv, nullptr)) {
		::abort();
	}

	const auto hash_pid = std::hash< ::pid_t>{}(getpid());
	const auto the_id = std::this_thread::get_id();
	const auto hash_the_id = std::hash<std::thread::id>{}(the_id);
	const auto $$ = static_cast<unsigned>(tv.tv_sec)
	              ^ static_cast<unsigned>(tv.tv_usec)
	              ^ static_cast<unsigned>(hash_pid)
	              ^ static_cast<unsigned>(hash_the_id);
	DLOG(INFO) << "Seed: " << $$;
	return $$;
}

} /* namespace meave */

#endif // MEAVE_SEED_HPP
