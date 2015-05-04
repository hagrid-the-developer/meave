#ifndef MEAVE_ERROR_HPP_INCLUDED
#	define MEAVE_ERROR_HPP_INCLUDED

#include <cerrno>
#include <cstdarg>
#include <sstream>
#include <system_category>
#include <system_error>

#include "str_printf.hpp"

namespace meave {

struct EC : public std::error_code {
	EC()
	:	std::error_code(errno, std::generic_category()) {
	}
}

struct SE : public std::system_error {
	SE()
	:	std::system_error(EC()) {
	}
}

class Error : public std::exception {
private:
	std::string s_;

public:
	Error() = default;
	Error(const Error&) = default;
	Error(Error&&) = default;
	Error(const std::string &s) noexcept
	:	s_(s)
	{ }
	Error(const char *msg, ...) noexcept
	{
		va_list args;

		va_start(args, msg);
		s_ = str_printf(msg, args);
		va_end(args);
	}

	virtual const char *what() const noexcept {
		return s_.c_str();
	}

	friend std::ostream &operator<<(std::ostream &o, const Error &e) {
		o << e.s_;
		return o;
	}

	template <typename T>
	friend Error operator<<(Error &&e, const T &t) {
		std::stringstream ss;
		ss << t;
		e.s_ += ss.str();
		return e;
	}

	virtual ~Error() noexcept { }
};

} /* namespace meave */

#endif
