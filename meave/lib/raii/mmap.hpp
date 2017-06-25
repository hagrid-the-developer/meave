#ifndef MEAVE_RAII_MMAP_HPP_INCLUDED
#	define MEAVE_RAII_MMAP_HPP_INCLUDED

#include <unistd.h>

#include "raii/fd.hpp"
#include "error.hpp"

namespace meave { namespace raii {

	class MMap {
	private:
		const void *mem_;
		::size_t file_size_;

	public:
		MMap(const char *filename) throw(Error)
		: mem_(nullptr)
		, file_size_(0)
		{
			const raii::FD fd( ::open(filename, O_RDONLY) );

			if (*fd == -1)
				throw Error("Cannot open: %s: %m", filename);

			struct stat st;
			if (-1 == ::fstat(*fd, &st))
				throw Error("Cannot stat: %s: %m", filename);

			const ::size_t size = st.st_size;
			if (size != st.st_size)
				throw Error("File is too big: %s", filename);

			void *mem = ::mmap(0, size, PROT_READ, MAP_SHARED, *fd, 0);
			if (mem == MAP_FAILED)
				throw Error("Cannot mmap: %s: %m", filename);

			mem_ = mem;
			file_size_ = size;
		}

		MMap(const MMap&) = delete;
		MMap(MMap &&x)
		:	mem_(x.mem_)
		{
			x.mem_ = nullptr;
		}

		const void* operator*() const noexcept __attribute__((assume_aligned(4096))) {
			return mem_;
		}

		::size_t file_size() const noexcept {
			return file_size_;
		}

		MMap& operator=(MMap&) = delete;
		MMap& operator=(MMap&&x) {
			mem_ = x.mem_;
			x.mem_ = nullptr;

			return *this;
		}

		~MMap() noexcept {
			if (mem_ != nullptr)
				::munmap(const_cast<void*>(mem_), file_size_);
		}
	};

} } /* namespace meave::raii */


#endif // MEAVE_RAII_MMAP_HPP_INCLUDED
