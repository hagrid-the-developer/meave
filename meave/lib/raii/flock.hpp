#ifndef MEAVE_RAII_FLOCK_HPP_INCLUDED
#	define MEAVE_RAII_FLOCK_HPP_INCLUDED

#include <sys/file.h>
#include <system_error>
#include <unistd.h>

namespace meave { namespace raii {

class Flock {
private:
	int fd_;

	static bool lock(const int fd, const int operation) throw(std::system_error) {
		const int nb = operation & LOCK_NB;
		const int op = (operation & ~LOCK_NB) == LOCK_SH ? LOCK_SH : LOCK_EX;
		while (flock(fd, op | nb) == -1) {
			switch (errno) {
			case EWOULDBLOCK:
				return false;
			case EINTR:
				break; // Repeat the cycle once again
			default:
				throw ::meave::SE();
			}
		}
		return true;
	}

	static void unlock(const int fd) noexcept {
		while (flock(fd, LOCK_UN) == -1) {
			if (errno != EINTR) {
				// Cannot release lock: %m
			}
		}
	}

public:
	Flock(const FD &fd, const int operation) throw(Error)
	:	fd_(lock(*fd, operation) ? *fd : -1) {
	}

	bool is_locked() const noexcept {
		return fd_ != -1;
	}
	explicit operator bool() const {
		return is_locked();
	}

	Flock(const Flock&) = delete;
	Flock(Flock &&$)
	:	fd_($.fd_) {
		$.fd_ = -1;
	}

	int operator*() const noexcept {
		return fd_;
	}

	Flock& operator=(const Flock&) = delete;
	Flock& operator=(Flock &&$) {
		fd_ = $.fd_;
		$.fd_ = -1;

		return *this;
	}

	~Flock() noexcept {
		if (is_locked())
			unlock(fd_);
	}
};

} } /* namespace meave::raii */

#endif // MEAVE_RAII_FLOCK_HPP_INCLUDED
