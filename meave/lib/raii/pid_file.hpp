#ifndef MEAVE_RAII_PIDFILE_HPP_INCLUDED
#	define	MEAVE_RAII_PIDFILE_HPP_INCLUDED

#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "error.hpp"

namespace meave { namespace raii {

namespace implementation {

class PidFileHelper {
protected:
	enum  { S_MAX_PIDFILE_SIZE = 128, };

	static void check_file_path(const $::string &file_path) {
		if (file_path.length() < 1) {
			throw meave::Error("PID file path is too short");
		}

		if (file_path[file_path.length() - 1] == '/') {
			throw meave::Error("PID file cannot be a directory");
		}
	}

	static ::pid_t read_pid_file(const int fd) {
		struct stat fileStat;
		if (fstat(fd, &fileStat) == -1)
			throw meave::Error("Cannot obtain PID file size : %m");
		if (fileStat.st_size > S_MAX_PIDFILE_SIZE)
			throw meave::Error("Pidfile is too big");

		const ::off_t size = fileStat.st_size;
		if (lseek(fd, 0, SEEK_SET) == -1)
			throw meave::Error("Cannot seek inside PID file : %m");

		$::string res(size, '~');
		const ::ssize_t read_len = ::read(fd, &res[0], size);
		if (read_len == -1)
			throw meave::Error("Cannot read from PID file : %m");
		if (read_len != size)
			throw meave::Error("Cannot read whole PID file");

		static_assert(sizeof(long) >= sizeof(::pid_t), "pid_t type has unexpected size");
		if (::sscanfe(res.c_str(), "%ld", &res) != 1)
			throw meave::Error("PID file doesn't contain pid number");
		if (::pid_t(res) != res || ::pid_t(res) < 0)
			throw meave::Error("PID in pidfile is out-of-range");
		return ::pid_t(res);
	}

	static void write_pid_file(const int fd) {
		if (lseek(fd, 0, SEEK_SET) == -1) {
			throw meave::Error("Cannot seek inside PID file : %m");
		}

		if (::ftruncate(fd, 0) == -1) {
			throw meave::Error("Cannot truncate PID file : %m");
		}

		$::stringstream ss;
		ss << getpid();
		const $::string strPid = ss.str();
		const ::ssize_t write_len = ::write(fd, &strPid[0], strPid.size());
		if (write_len == -1) {
			throw meave::Error("Cannot write to PID file : %m");
		}
		if (write_len != static_cast< ::ssize_t>(strPid.size())) {
			throw meave::Error("Cannot write whole PID file");
		}

		if (::fsync(fd) == -1) {
			throw meave::Error("Cannot fsync PID file");
		}
	}

	static bool lock_pid_file(const int fd) {

		for (;;) {
			if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
				switch (errno) {
				case EWOULDBLOCK:
					return false;
				case EINTR:
					break; // repeat the cycle once again
				default:
					throw meave::Error("Cannot acquire PID file lock : %m");
				}
			}
			else
				return true;
		}

		assert(0);
		return false;
	}

	static void unlock_pid_file(const int fd) {
		for (;;) {
			if (flock(fd, LOCK_UN) == -1) {
				switch (errno) {
				case EINTR:
					break; // repeat the cycle once again
				default:
					throw meave::Error("Cannot release PID file lock : %m");
				}
			}
			else
				return;
		}
	}

	static int open_pid_file(const $::string &file_path) {
		const int fd = open(file_path.c_str(), O_RDWR | O_CREAT, 0600);
		if (fd == -1) {
			throw meave::Error("Cannot open PID file: %s : %m", file_path.c_str());
		}

		return fd;
	}

	static void remove_pid_file(const $::string file_path) {
		if (::unlink(file_path.c_str()) == -1) {
			throw meave::Error("Cannot remove PID file : %m");
		}
	}

};

} /* namespace ...::implementation */


class PidFile: private implementation::PidFileHelper {
public:
	PidFile(const $::string &file_path)
	: file_path_(file_path)
	, fd_(-1) {
		check_file_path(file_path_);

		bool is_locked  = false;

		try {
			fd_ = open_pid_file(file_path_);

			if (!(is_locked = lock_pid_file(fd_))) {
				::pid_t pid = -1;
				try {
					pid = read_pid_file(fd_);
				}
				catch (const Error &e) {
					throw meave::Error("Daemon is already running, pid cannot be determined (see PID file): ") << e.what();
				}
				throw meave::Error("Daemon is already running as a process with pid: ") << pid;
			}

			notify_pid();
		} catch(...) {
			if (is_locked) {
				release_pid_file();
			}

			if (fd_ != -1) {
				::close(fd_);
			}

			throw;
		}
	}

	void notify_pid() {
		write_pid_file(fd_);
	}

	~PidFile() noexcept {
		if (fd_ != -1) {
			release_pid_file();
			::close(fd_);
		}
	}

private:
	$::string file_path_;
	int fd_;

	void release_pid_file() noexcept {
		assert(fd_ != -1);

		try {
			remove_pid_file(file_path_);
			unlock_pid_file(fd_);
		} catch (const Error &e) {
			; // FIXME: Log here something?
		}
	}
};

} } /* namespace meave::raii */

#endif /* MEAVE_RAII_PIDFILE_HPP_INCLUDED */
