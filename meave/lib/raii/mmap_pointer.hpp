#ifndef MEAVE_RAII_MMAP_POINTER_HPP_INCLUDED
#	define MEAVE_RAII_MMAP_POINTER_HPP_INCLUDED

#include <cstdlib>
#include <meave/lib/error.hpp>
#include <meave/lib/raii/mmap_mem.hpp>

namespace meave { namespace raii {

template<typename T>
class MMapPointer {
private:
	MMapMem p_;

public:
	MMapPointer() noexcept
	:	p_(0)
	{ }
	MMapPointer(const ::size_t len, const int mmap_flags = 0) throw(Error)
	:	p_(len * sizeof(T), mmap_flags)
	{ }
	MMapPointer(const MMapPointer &) = delete;
	MMapPointer(MMapPointer &&x) = default;

	MMapPointer &operator=(const MMapPointer &) = delete;
	MMapPointer &operator=(MMapPointer &&x) = default;

	T *operator*() noexcept __attribute__((assume_aligned(4096))) {
		return reinterpret_cast<T*>(*p_);
	}

	const T *operator*() const noexcept __attribute__((assume_aligned(4096))) {
		return reinterpret_cast<const T*>(*p_);
	}

	T &operator[](const ::size_t index) noexcept {
		return (**this)[index];
	}

	const T &operator[](const ::size_t index) const noexcept {
		return (**this)[index];
	}

	::size_t size() const noexcept {
		return p_.size();
	}

	::size_t len() const noexcept {
		return size()/sizeof(T);
	}

	~MMapPointer() = default;
};

} } /* namespace meave::raii */

#endif // MEAVE_RAII_MMAP_POINTER_HPP_INCLUDED
