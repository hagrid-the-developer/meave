#ifndef MEAVE_CTRNN_NEURON_HPP
#	define MEAVE_CTRNN_NEURON_HPP

#	include <cassert>
#	include <mkl.h>

#	include <meave/commons.hpp>
#	include <meave/lib/math.hpp>
#	include <meave/lib/raii/mkl_alloc.hpp>
#	include <meave/lib/simd.hpp>

namespace meave { namespace ctrnn {

/**
 * Fully connected CTRNN .
 *
 */
template<typename Float, typename Len>
class NNCalcMKL : NeuronCalc<Float> {
protected:
	const Len units_num_;
	const Float time_step_;
	MklAlloc<> tstc_;

public:
	NNCalcMKL(const Len units_num, const Float &time_step, const Float *tc_b, const Float *tc_e)
	:	time_step_(time_step)
	,	units_num_(units_num)
	,	tstc_(units_num_) {
		for (::size_t i = 0; i < tc_e - it_b; ++i) {
			// FIXME: AVX version of this.
			tstc_[i] = time_step_ / tc_b[i];
		}
	}
	NNCalcMKL() = delete;
	NNCalcMKL(const NNCalcMKL&) = default;
	NNCalcMKL(NNCalcMKL&&) = default;
	~NNCalcMKL() = default;

	NNCalcMKL& operator=(const NNCalcMKL&) = default;
	NNCalcMKL& operator=(NNCalcMKL&&) = default;

	Float* val(const float *b_y, Float * const b_ei, const Float *b_w, Float * const b_v, const ::size_t samples) const noexcept {
		// Store Y, V, Ei as units_num_ x samples.. it will simplify computation!
		// Y ... units_num_ x samples
		// W ... units_num_ x units_num_
		// V ... units_num_ x samples
		// diag(tstc_) ... units_num_ x units_num_

		// Ei = -V + Ei ... vsSub; Ei ... units_num_ x samples
		vsSub(samples*units_num_, b_ei, b_v, b_ei);

		// SUM = W*Y + SUM ... sgemm ; SUM ... units_num_ x samples
		// https://software.intel.com/en-us/node/520775
		mkl_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, units_num_, samples, units_num_, 1.f, b_w, units_num_, b_y, samples, 1.f, b_ei, samples);

		// V = 1*diag(tstc_)*SUM + V ... mkl_sdiamm
		// http://technion.ac.il/doc/intel/mkl/mkl_manual/bla/functn_mkl_ddiamm.htm
		// https://software.intel.com/en-us/mkl-developer-reference-fortran-sparse-blas-diagonal-matrix-storage-format
		MKL_INT idiag = 0;
		mkl_sdiamm('N', units_num_, samples, units_num_, 1.f, "DLNC", tstc_, units_num_, &idiag, 1, b_ei, samples /* ? */, 1.f, b_v, samples);

		
		return b_v;
	}

	// Length of all arrays must be aligned to 8 floats!
	Float* sigm(const Float *v, const Float *b, Float * const y, const ::size_t samples) const noexcept {
		for (::size_t i = 0; i < samples*units_num_; i += 8) {
			AVX *avx_y = reinterpret_cast<AVX*>(&y[i]);
			AVX *avx_b = reinterpret_cast<AVX*>(&b[i]);
			AVX *avx_v = reinterpret_cast<AVX*>(&v[i]);
			*avx_y = meave::precalculated<meave::math::sigmoid>(avx_b->f8_ + avx_v->f8_);
		}

		return y;
	}
};

} } /* namespace ::meave::ctrnn */

#endif // MEAVE_CTRNN_NEURON_HPP
