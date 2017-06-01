#ifndef MEAVE_RAII_ACCUMULATE_FLUSH_HPP_INCLUDED
#	define MEAVE_RAII_ACCUMULATE_FLUSH_HPP_INCLUDED

#include "meave/commons.hpp"
#include "meave/lib/error.hpp"
#include "meave/lib/raii/fd.hpp"
#include "meave/lib/vec_of_pods.hpp"

#include <fcntl.h>
#include <string>
#include <utility>
#include <sys/types.h>
#include <sys/stat.h>

namespace meave { namespace raii {

template <typename PodT>
class AccumulateFlush {
private:
	$::string file_name_;
	VecOfPods<PodT> pods_;

	void flush() {
		FD fd{::creat(file_name_.c_str(), 0600)};
		if (!fd)
			throw Error("Cannot open file: %s: %m", file_name_.c_str());

		const ::uint8_t *p = pods_.u8();
		::size_t len = pods_.size() * sizeof(PodT);
		while(len) {
			const ::ssize_t written = ::write(*fd, p, len);
			if (written < 0) {
				if (errno == EINTR)
					continue;
				throw Error("Cannot write to file: %s: %m", file_name_.c_str());
			}
			p += ::size_t(written);
			len -= ::size_t(written);
		}
	}

public:
	AccumulateFlush(const $::string &file_name)
	:	file_name_(file_name) {
	}
	AccumulateFlush() = delete;
	AccumulateFlush(const AccumulateFlush&) = delete;

	template <typename ...Args>
	void operator()(Args &&...args) {
		pods_.emplace_back($::forward<Args>(args)...);
	}

	~AccumulateFlush() noexcept {
		try {
			flush();
		} catch (const Error &e) {
			LOG(ERROR) << "Cannot flush: `" << file_name_ << "' : " << e.what();
		} catch (...) {
			LOG(ERROR) << "Cannot flush: `" << file_name_ << "'";
		}
	}
};

} } /* namespace meave::raii */ 

#endif
