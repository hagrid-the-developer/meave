#ifndef MEAVE_LIB_VEC_OF_PODS_HPP_INCLUDED
#	define MEAVE_LIB_VEC_OF_PODS_HPP_INCLUDED

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

namespace meave {

// FIXME: Make this standard STL-container
template<typename T>
class VecOfPods {
//static_assert(std::is_pod<T>::value, "Only PODs are supported")
private:
	T *ptr_;
	// len_ < 0 means not-to-free
	::size_t len_;
	::size_t capacity_;

public:
	VecOfPods() noexcept
	:	VecOfPods(nullptr, 0) {
	}
	VecOfPods(T *ptr, const ::size_t len) noexcept
	:	ptr_(ptr)
	,	len_(::size_t(len))
	,	capacity_(0) {
		assert(!!ptr == !!len);
	}
	VecOfPods(const VecOfPods&) = delete;
	VecOfPods(VecOfPods &&vec) noexcept
	:	ptr_(vec.ptr_)
	,	len_(vec.len_)
	,	capacity_(vec.capacity_) {
		vec.ptr_ = nullptr;
		vec.len_ = 0;
		vec.capacity_ = 0;
	}

	VecOfPods &operator=(const VecOfPods&) = delete;
	VecOfPods &operator=(VecOfPods &&vec) noexcept {
		ptr_ = vec.ptr_;
		len_ = vec.len_;
		capacity_ = vec.capacity_;
		
		vec.ptr_ = nullptr;
		vec.len_ = 0;
		vec.capacity_ = 0;
	}

	::size_t len() const noexcept {
		return len_;
	}
	::size_t size() const noexcept {
		return len();
	}
	::size_t capacity() const noexcept {
		return capacity_;
	}

	T &operator[](const ::size_t i) const noexcept {
		assert(i < len_);
		return ptr_[i];
	}

	void reserve(const ::size_t n) noexcept {
		if (n <= capacity())
			return;
		if (!capacity()) {
			const ::size_t new_capacity = std::max(n, 2*len());
			T *t = reinterpret_cast<T*>(::malloc(new_capacity * sizeof(T)));
			if (!t)
				::abort();
			::memcpy(t, ptr_, len() * sizeof(T));
			ptr_ = t;
			capacity_ = n;
		} else {
			const ::size_t new_capacity = std::max(n, 2*capacity());
			T *t = reinterpret_cast<T*>(::realloc(ptr_, new_capacity*sizeof(T)));
			if (!t)
				::abort();
			ptr_ = t;
			capacity_ = new_capacity;
		}
	}
	void resize(const ::size_t size) noexcept {
		reserve(size);
		len_ = size;
	}
	void resize(const ::size_t size, const T &val) noexcept {
		reserve(size);
		while(len_ < size) {
			ptr_[len_++] = val;
		}
	}

	T* begin() noexcept {
		return &ptr_[0];
	}
	const T* begin() const noexcept {
		return &ptr_[0];
	}
	T* end() noexcept {
		return &ptr_[len()];
	}
	const T* end() const noexcept {
		return &ptr_[len()];
	}


	T *data() noexcept {
		return begin();
	}

	::uint8_t *u8() noexcept {
		return reinterpret_cast<::uint8_t*>(data());
	}
	const ::uint8_t *u8() const noexcept {
		return reinterpret_cast<const ::uint8_t*>(data());
	}
	void push_back(const T &t) {
		reserve(len_ + 1);
		new (&ptr_[len_]) T(t);
		++len_;
	}
	void push_back(T &&t) {
		reserve(len_ + 1);
		new (&ptr_[len_]) T(std::forward<T>(t));
		++len_;
	}

	template <typename ...Args>
	void emplace_back(Args &&...args) {
		reserve(len_ + 1);
		new (&ptr_[len_]) T(std::forward<Args>(args)...);
		++len_;
	}

	~VecOfPods() noexcept {
		if (capacity() > 0) {
			assert(ptr_);
			::free(ptr_);
		}
	}
};

} /* namespace meave */ 

#endif // MEAVE_LIB_VEC_OF_PODS_HPP_INCLUDED
