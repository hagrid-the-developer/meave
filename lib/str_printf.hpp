#ifndef MEAVE_STR_PRINTF_HPP_INCLUDED
#	define MEAVE_STR_PRINTF_HPP_INCLUDED

#include <cassert>
#include <cstdarg>
#include <string>

namespace meave {

std::string
str_printf(const char *msg, va_list args) noexcept
{
	va_list args2;

	va_copy(args2, args);
	const int len = ::vsnprintf(nullptr, 0, msg, args2);
	va_end(args2);

	std::string ret(len + 1, '~');
	::vsnprintf(&ret[0], len + 1, msg, args);
	assert(ret.back() == 0);
	ret.resize(len);

	return ret;
}

std::string
str_printf(const char *msg, ...) noexcept
{
	va_list args;

	va_start(args, msg);
	std::string ret = str_printf(msg, args);
	va_end(args);
	return ret;
}

} /* namespace meave */

#endif
