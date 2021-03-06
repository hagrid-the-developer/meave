#ifndef MEAVE_RAII_FD_HPP_INCLUDED
#	define MEAVE_RAII_FD_HPP_INCLUDED

#include <unistd.h>

namespace meave { namespace raii {

	class FD {
	private:
		int fd_;

	public:
		FD(const int fd = -1) noexcept
		:	fd_(fd)
		{ }
		FD(const FD&) = delete;
		FD(FD &&x)
		:	fd_(x.fd_)
		{
			x.fd_ = -1;
		}

		int operator*() const noexcept {
			return fd_;
		}

		FD& operator=(const FD&) = delete;
		FD& operator=(FD&&x) {
			fd_ = x.fd_;
			x.fd_ = -1;

			return *this;
		}

		bool is_opened() const noexcept {
			return fd_ != -1;
		}
		explicit operator bool() const {
			return is_opened();
		}

		~FD() noexcept {
			if (fd_ != -1)
				::close(fd_);
		}
	};

} } /* namespace meave::raii */


#endif
