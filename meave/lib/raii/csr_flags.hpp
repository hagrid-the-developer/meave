#ifndef MEAVE_RAII_CSR_FLAGS_HPP_INCLUDED
#	define MEAVE_RAII_CSR_FLAGS_HPP_INCLUDED

namespace meave { namespace raii {

	template<int NEW_FLAGS>
	class CSRFlags {
	private:
		int flags_;

	public:
		CSRFlags() noexcept
		:	flags_( ::_mm_getcsr())
		{
			::_mm_setcsr(flags_ | NEW_FLAGS);
		}

		CSRFlags(const CSRFlags&) = delete;
		CSRFlags(CSRFlags &&x) = delete;

		CSRFlags& operator=(CSRFlags&) = delete;
		CSRFlags& operator=(CSRFlags&&x) = delete;

		~CSRFlags() noexcept {
			::_mm_setcsr(flags_);
		}
	};

	// Flush-To-Zero and Denormals-are-Zero
	typedef CSRFlags<0x0040 | 0x8000> CSRFlagsAvoidDenormals;

} } /* namespace meave::raii */


#endif // MEAVE_RAII_CSR_FLAGS_HPP_INCLUDED
