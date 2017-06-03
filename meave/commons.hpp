#ifndef MEAVE_COMMONS_HPP
#   define MEAVE_COMMONS_HPP

namespace std { }

namespace boost {
	namespace asio {
	}
}

namespace b = boost;
namespace nio = boost::asio;

namespace $ = std;

typedef unsigned uns;

/* Assertions. */
#include <glog/logging.h>

#ifdef NDEBUG
#	define MEAVE_ASSERT($ignored) ((void)0)
#else
#	define MEAVE_ASSERT($cond) \
		do { \
			if (!($cond)) \
				LOG(FATAL) << "Assertion failed:" << #$cond << "; f:" << __FILE__ << "; l:" << __LINE__; \
		} while(0)
#endif


/* Macro for defining simple classes that are used to transfer parameters
 *   to functions.
 * It helps to avoid mistakes caused by misplaced parameters. */

#define PARAM_CLASS(name)					\
	template<typename T>					\
	class name {						\
	private:						\
		T t_;						\
								\
	public:							\
		constexpr name(const T& t) noexcept 		\
		:	t_(t) {					\
		}						\
								\
		constexpr T operator*() const noexcept {	\
			return t_;				\
		}						\
	};


#include <future>
namespace meave {
template<typename T1, typename T2, typename F>
void for_xrange(const T1 __first, const T2 __last, F &&__f) {
	for (T2 __i = __first; __i != __last; ++__i) {
		__f(__i);
	}
}

namespace par {
template<typename T1, typename T2, typename F>
void for_xrange(const T1 __first, const T2 __last, F &&__f) {
	const uns __THREADS_NUM = 8;

	std::future<void> __futures[__THREADS_NUM];

	const uns __len = __last - __first + 1;
	for (uns __th_id = 0; __th_id < __THREADS_NUM; ++__th_id) {
		const auto __from = __first + __th_id*__len/__THREADS_NUM;
		const auto __to =  __first + (__th_id + 1)*__len/__THREADS_NUM;
		__futures[__th_id] = $::move($::launch::async, $::async([__from, __to, __f]() {
			meave::for_xrange(__from, __to, __f);
		}));
	}
	for (uns __th_id = 0; __th_id < __THREADS_NUM; ++__th_id) {
		__futures[__th_id].wait();
	}
}

} /* namespace meave::par */

static inline constexpr uns cache_size() {
#if defined(__x86_64)
	return 64U;
#else
#error Unknown architecture
#endif
}

} /* namespace meave */

#endif // MEAVE_COMMONS_HPP
