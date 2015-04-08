#ifndef MEAVE_ERROR_HPP_INCLUDED
#	define MEAVE_ERROR_HPP_INCLUDED

#include <cstdarg>

#include "str_printf.hpp"

namespace meave {

class Error : public std::exception {
	private:
		std::string s_;

	public:
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

		virtual ~Error() noexcept { }
};

} /* namespace meave */

#endif
