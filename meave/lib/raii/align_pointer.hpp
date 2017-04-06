#ifndef MEAVE_RAII_ALIGN_POINTER_HPP_INCLUDED
#	define MEAVE_RAII_ALIGN_POINTER_HPP_INCLUDED

#include <cstdlib>

#include "error.hpp"

namespace meave { namespace raii {

template<typename T, int ALIGNMENT>
class AlignPointer {
private:
	T *p_;

public:
	AlignPointer() noexcept
	:	p_(nullptr)
	{ }
	AlignPointer(const ::size_t len) throw(Error)
	:	AlignPointer()
	{
		const ::size_t size = len * sizeof(T);
		if ( (errno = ::posix_memalign(reinterpret_cast<void**>(&p_), ALIGNMENT, size)) ) {
			throw Error("Cannot allocate aligned memory: aling:%d size:%zu : %m", ALIGNMENT, size);
		}
	}
	AlignPointer(const AlignPointer &) = delete;
	AlignPointer(AlignPointer &&x) noexcept {
		p_ = x.p_;
		x.p_ = nullptr;
	}

	AlignPointer &operator=(const AlignPointer &) = delete;
	AlignPointer &operator=(AlignPointer &&x) noexcept {
		p_ = x.p_;
		x.p_ = nullptr;
	}

	T *operator*() {
		return p_;
	}

	const T *operator*() const {
		return p_;
	}

	T &operator[](const ::size_t index) {
		return p_[index];
	}

	const T &operator[](const ::size_t index) const {
		return p_[index];
	}

	~AlignPointer() {
		if (p_)
			::free(p_);
	}
};

} } /* namespace meave::raii */

#endif // MEAVE_RAII_ALIGN_POINTER_HPP_INCLUDED
