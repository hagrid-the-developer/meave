#ifndef MEAVE_RAII_MMAP_MEM_HPP_INCLUDED
#	define MEAVE_RAII_MMAP_MEM_HPP_INCLUDED

#include <sys/mman.h>
#include <unistd.h>

#include <meave/lib/error.hpp>

namespace meave { namespace raii {

	class MMapMem {
	private:
		void *mem_;
		::size_t size_;

	public:
		MMapMem(const ::size_t size, const int flags = 0) throw(Error)
		: mem_(nullptr)
		, size_(0) {
			if (!size)
				return;

#ifdef __linux__
			enum { ALLOWED_FLAGS = MAP_HUGETLB | MAP_LOCKED };
#else
			enum { ALLOWED_FLAGS = 0 };
#endif
			assert( !(flags & ~ALLOWED_FLAGS) );

			const int pt = PROT_READ | PROT_WRITE;
			const int fl = MAP_PRIVATE | MAP_ANONYMOUS | flags;

			void *mem = ::mmap(0, size, pt, fl, -1, 0);
			if (mem == MAP_FAILED)
				throw Error("Cannot mmap memory of size: %zu: %m", size);

			mem_ = mem;
			size_ = size;
		}

		MMapMem(const MMapMem&) = delete;
		MMapMem(MMapMem &&x)
		:	mem_(x.mem_)
		,	size_(x.size_) {
			x.mem_ = nullptr;
			x.size_ = 0;
		}

		const void* operator*() const noexcept {
			return mem_;
		}
		void* operator*() noexcept {
			return mem_;
		}

		::size_t size() const noexcept {
			return size_;
		}

		MMapMem& operator=(MMapMem&) = delete;
		MMapMem& operator=(MMapMem&&x) {
			mem_ = x.mem_;
			size_ = x.size_;

			x.mem_ = nullptr;
			x.size_ = 0;

			return *this;
		}

		~MMapMem() noexcept {
			if (mem_ != nullptr)
				::munmap(const_cast<void*>(mem_), size_);
		}
	};

} } /* namespace meave::raii */


#endif // MEAVE_RAII_MMAP_MEM_HPP_INCLUDED
