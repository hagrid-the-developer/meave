#ifndef MEAVE_RAII_FORK_HPP_INCLUDED
#	define MEAVE_RAII_FORK_HPP_INCLUDED

#	include <cstdlib>
#	include <limits>
#	include <unistd.h>

#	include "meave/commons.hpp"
#	include "meave/lib/error.hpp"

namespace meave { namespace raii {

static_assert($::numeric_limits< ::pid_t>::max() <= $::numeric_limits<int>::max(), "::pid_t > int");
static_assert($::numeric_limits< ::pid_t>::min() >= $::numeric_limits<int>::min(), "::pid_t < int");

class Fork {
private:
	int pid_;

public:
	Fork()
	:	pid_(-1) {
	}
	template<typename Fn, typename... Args>
	explicit Fork(Fn &&fn, Args&&... args) throw(std::system_error) {
		pid_ = ::fork();
		switch (pid_) {
		case 0: {
			const int ret = fn(args...);
			::exit(ret);
		}

		case -1:
			throw meave::SE();

		default:
			return;
		}
	}
	/**
	 * Ooops, we are quite controversial here.
	 *   We have a constructor, that returns non-empty error_code
	 *   object in case of error and leaves its object in "empty"
	 *   state. But in this case it seems it has sense and it works.
	 */
	template<typename Fn, typename... Args>
	Fork(std::error_code &ec, Fn &&fn, Args&&... args) noexcept {
		pid_ = ::fork();
		switch (pid_) {
		case 0: {
			const int ret = fn(args...);
			::exit(ret);
		}

		case -1:
			ec = meave::EC();
			return;

		default:
			return;
		}
	}
	Fork(const Fork&) = delete;
	Fork(Fork &&$)
	:	pid_($.pid_) {
		$.pid_ = -1;
	}
	Fork &operator=(const Fork&) = delete;
	Fork &operator=(Fork &&$) noexcept {
		pid_ = $.pid_;
		$.pid_ = -1;

		return *this;
	}

	int pid() const noexcept {
		return pid_;
	}

	int wait() throw(std::system_error) {
		assert(pid_ != -1);

		int status;
		while (-1 == waitpid(pid_, &status, 0)) {
			if (errno != EINTR)
				throw SE();
		}
		pid_ = -1;
		return status;
	}
	int wait(std::error_code &ec) noexcept {
		assert(pid_ != -1);

		int status;
		while (-1 == waitpid(pid_, &status, 0)) {
			if (errno != EINTR) {
				ec = EC();
				return 0;
			}
		}
		pid_ = -1;
		return status;
	}

	~Fork() noexcept {
		if (pid_ != -1) {
			std::error_code ec;
			wait(ec);
//			if (ec)
//				LOG(ERROR) << "Wait failed" << ec.message()
		}
	}
};

} } /* namespace mave::raii */

#endif // MEAVE_RAII_FORK_HPP_INCLUDED
