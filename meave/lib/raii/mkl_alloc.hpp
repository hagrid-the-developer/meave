#ifndef MEAVE_RAII_MKL_ALLOC_HPP_INCLUDED
#	define MEAVE_RAII_MKL_ALLOC_HPP_INCLUDED

#include <sys/mman.h>
#include <unistd.h>

#include <meave/lib/error.hpp>

namespace meave { namespace raii {

	template<typename T=float, int ALIGNMENT=64>
	class MklAlloc {
	private:
		void *mem_;

	public:
		MklAlloc(const ::size_t size) throw(Error)
		: mem_(nullptr) {
			if (!size)
				return;

			mem_ = ::mkl_alloc(size, ALIGNMENT);
		}

		MklAlloc(const MklAlloc&) = delete;
		MklAlloc(MklAlloc &&x)
		:	mem_(x.mem_) {
			x.mem_ = nullptr;
			x.size_ = 0;
		}

		const T* operator*() const noexcept __attribute__((assume_aligned(ALIGNMENT))) {
			return reinterpret_cast<const T*>(mem_);
		}
		T* operator*() noexcept __attribute__((assume_aligned(ALIGNMENT))) {
			return reinterpret_cast<T*>(mem_);
		}

		MklAlloc& operator=(MklAlloc&) = delete;
		MklAlloc& operator=(MklAlloc&&x) {
			mem_ = x.mem_;
			x.mem_ = nullptr;
			return *this;
		}

		~MklAlloc() noexcept {
			::mkl_free(mem_);
		}
	};

} } /* namespace meave::raii */


#endif // MEAVE_RAII_MKL_ALLOC_HPP_INCLUDED
