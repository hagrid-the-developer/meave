#ifndef MEAVE_RAII_MMAP_CREATE_HPP_INCLUDED
#	define MEAVE_RAII_MMAP_CREATE_HPP_INCLUDED

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "meave/lib/raii/fd.hpp"
#include "meave/lib/error.hpp"

namespace meave { namespace raii {

	class MMapCreate {
	private:
		void *mem_;
		::size_t file_size_;

	public:
		MMapCreate(const char *filename, const ::size_t file_size)
		: mem_(nullptr)
		, file_size_(file_size)
		{
			/* MAP_SHARED and PROT_WRITE require O_RDWR */
			const raii::FD fd{ ::open(filename, O_RDWR | O_TRUNC | O_CREAT, 0600) };

			if (!fd)
				throw Error("Cannot open: %s: %m", filename);

			if (-1 == ::ftruncate(*fd, ::off_t(file_size)))
				throw Error("Cannot resize file %s: %m", filename);

			void *mem = ::mmap(0, file_size, PROT_WRITE, MAP_SHARED, *fd, 0);
			if (mem == MAP_FAILED)
				throw Error("Cannot mmap: %s: %m", filename);

			mem_ = mem;
		}

		MMapCreate(const MMapCreate&) = delete;
		MMapCreate(MMapCreate &&x)
		:	mem_(x.mem_) {
			x.mem_ = nullptr;
			x.file_size_ = 0;
		}

		void* operator*() noexcept __attribute__((assume_aligned(4096))) {
			return mem_;
		}
		const void* operator*() const noexcept __attribute__((assume_aligned(4096))) {
			return mem_;
		}

		::size_t file_size() const noexcept {
			return file_size_;
		}

		MMapCreate& operator=(MMapCreate&) = delete;
		MMapCreate& operator=(MMapCreate&&x) {
			mem_ = x.mem_;
			file_size_ = x.file_size_;

			x.mem_ = nullptr;
			x.file_size_ = 0;

			return *this;
		}

		~MMapCreate() noexcept {
			if (mem_ != nullptr)
				::munmap(const_cast<void*>(mem_), file_size_);
		}
	};

} } /* namespace meave::raii */


#endif // MEAVE_RAII_MMAP_CREATE_HPP_INCLUDED
