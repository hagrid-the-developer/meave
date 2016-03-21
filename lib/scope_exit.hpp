#ifndef MEAVE_SCOPE_EXIT_HPP_INCLUDED
#define MEAVE_SCOPE_EXIT_HPP_INCLUDED

#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <time.h>

namespace meave {

namespace {

template <typename F>
struct ScopeExit {
	F f_;

	ScopeExit(F f) : f_(f) {}
	~ScopeExit() { f_(); }
};

template <typename F>
ScopeExit<F> make_scope_exit(F f) {
	return ScopeExit<F>(f);
}

} /* anonymous namespace */

} /* namespace meave */

#define SCOPE_EXIT(code) \
	auto scope_exit_ ## __LINE__ = ::meave::make_scope_exit([=](){code;});

#endif // MEAVE_SCOPE_EXIT_HPP_INCLUDED
