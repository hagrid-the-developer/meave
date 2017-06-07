#ifndef MEAVE_CTRNN_NEURON_HPP
#	define MEAVE_CTRNN_NEURON_HPP

#	include <cassert>

#	include "meave/commons.hpp"
#	include "meave/lib/math.hpp"
#	include "meave/lib/simd.hpp"

namespace meave { namespace ctrnn {

PARAM_CLASS(TimeStep)

template<typename Float>
class NeuronCalc {
protected:
	const Float time_step_;

public:
	NeuronCalc(const TimeStep<Float> &time_step)
	:	time_step_(*time_step)
	{ }

	Float time_step() const noexcept {
		return time_step_;
	}

	template <typename ItY, typename ItW>
	Float val(const Float y, const Float time_constant, const Float ext_inp, const ItY b_y, const ItY e_y, const ItW b_w, const ItW e_w) const noexcept {
		assert($::distance(b_w, e_w) == $::distance(b_y, e_y));

		Float sum = 0;
		ItY it = b_y;
		ItW jt = b_w;

		for (; it != e_y; ++it, ++jt) {
			MEAVE_ASSERT(jt != e_w);
			sum += *it * *jt;
		}

		return y + time_step_*(-y + sum + ext_inp)/time_constant;
	}

	Float sigm(const Float val, const Float bias) const noexcept {
		return meave::math::sigmoid(bias + val);
	}

	template <typename ItY, typename ItW>
	Float operator()(const Float y, const Float ext_inp, const ItY b_y, const ItY e_y, const ItW b_w, const ItW e_w) const noexcept {
		return sigm(val(y, ext_inp, b_y, e_y, b_w, e_w));
	}
};

PARAM_CLASS(UnitsNum)

/**
 * Fully connected CTRNN .
 *
 */
template<typename Float, typename Len>
class NNCalc : NeuronCalc<Float> {
protected:
	const Len units_num_;

public:
	NNCalc(const UnitsNum<Len> &units_num, const TimeStep<Float> &time_step)
	:	NeuronCalc<Float>(time_step)
	,	units_num_(*units_num) {
	}

	template<typename ItY, typename ItTC, typename ItEI, typename ItW, typename ItV>
	ItV val(const ItY &b_y, const ItTC &b_tc, const ItEI &b_ei, const ItW &b_w, const ItV &b_v) const noexcept {
		ItTC it_tc = b_tc;
		ItW it_w = b_w;
		ItV it_v = b_v;
		ItEI it_ei = b_ei;
		for (Len val_idx = 0; val_idx != units_num_; ++val_idx) {
			Float sum = 0;
			ItY it_y = b_y;
			for (Len i = 0; i != units_num_; ++i) {
				//DLOG(INFO) << "sum[" << (it_v - b_v) << "] += " << *it_w << " * " << *it_y;
				sum += *it_w++ * *it_y++;
			}
			const Float v = *it_v; /* it_v will be incremented after assignement to it */
			const Float ei = *it_ei++;;
			//DLOG(INFO) << "next_val[" << (it_v - b_v) << "] = " << v << " + " << this->time_step_ << "* (" << -v << " + " << ei << " + " << sum << ") / " << *it_tc;
			*it_v++ = v + this->time_step_*(-v + ei + sum) / *it_tc++;
		}

		return b_v;
	}

	template<typename ItV, typename ItB, typename ItY>
	ItY sigm(const ItV &b_v, const ItB &b_b, const ItY &b_y) const noexcept {
		ItY it_y = b_y;
		ItB it_b = b_b;
		ItV it_v = b_v;

		for (Len i = 0; i != units_num_; ++i) {
			//DLOG(INFO) << "y[" << i << "] = sigm(" << -*it_b << " + " << *it_v << ")";
			*it_y++ = static_cast<const NeuronCalc<Float>&>(*this).sigm(*it_v++, *it_b++);
		}

		return b_y;
	}
};

class NeuronCalcAVX {
protected:
	using Float = float;
	using AVX = ::meave::simd::AVX;

	const Float time_step_;

public:
	NeuronCalcAVX(const TimeStep<Float> &time_step)
	:	time_step_(*time_step) {
	}

	Float time_step() const noexcept {
		return time_step_;
	}

	template <typename ItY, typename ItW>
	AVX val(const AVX y, const Float time_constant, const AVX ext_inp, const ItY b_y, const ItY e_y, const ItW b_w, const ItW e_w) const noexcept {
		assert($::distance(b_w, e_w) == $::distance(b_y, e_y));

		AVX sum = _mm256_setzero_ps();
		ItY it = b_y;
		ItW jt = b_w;

		for (; it != e_y; ++it, ++jt) {
			MEAVE_ASSERT(jt != e_w);
			const AVX x = _mm256_load_ps(&*it);
			const AVX w = _mm256_broadcast_ps(&*jt);
			sum.f8_ += x.f8_ * w.f8_;
		}

		return y.f8_ + time_step_*(-y.f8_ + sum.f8_ + ext_inp.f8_)/time_constant;
	}

	Float sigm(const Float val, const Float bias) const noexcept {
		// Add own implementation of exp(.)
		// Maybe use tables as is in the original code...
		return meave::math::sigmoid(bias + val);
	}

	template <typename ItY, typename ItW>
	Float operator()(const Float y, const Float ext_inp, const ItY b_y, const ItY e_y, const ItW b_w, const ItW e_w) const noexcept	{
		return sigm(val(y, ext_inp, b_y, e_y, b_w, e_w));
	}
};

/**
 * Fully connected CTRNN .
 *
 */
template<typename Float, typename Len>
class NNCalcAVX : NeuronCalc<Float> {
protected:
	const Len units_num_;

public:
	NNCalcAVX(const UnitsNum<Len> &units_num, const TimeStep<Float> &time_step)
	:	NeuronCalc<Float>(time_step)
	,	units_num_(*units_num)
	{ }

	template<typename ItY, typename ItTC, typename ItEI, typename ItW, typename ItV>
	ItV val(const ItY &b_y, const ItTC &b_tc, const ItEI &b_ei, const ItW &b_w, const ItV &b_v) const noexcept {
		ItTC it_tc = b_tc;
		ItW it_w = b_w;
		ItV it_v = b_v;
		ItEI it_ei = b_ei;
		for (Len val_idx = 0; val_idx != units_num_; ++val_idx) {
			Float sum = 0;
			ItY it_y = b_y;
			for (Len i = 0; i != units_num_; ++i) {
				//DLOG(INFO) << "sum[" << (it_v - b_v) << "] += " << *it_w << " * " << *it_y;
				sum += *it_w++ * *it_y++;
			}
			const Float v = *it_v; /* it_v will be incremented after assignement to it */
			const Float ei = *it_ei++;;
			//DLOG(INFO) << "next_val[" << (it_v - b_v) << "] = " << v << " + " << this->time_step_ << "* (" << -v << " + " << ei << " + " << sum << ") / " << *it_tc;
			*it_v++ = v + this->time_step_*(-v + ei + sum) / *it_tc++;
		}

		return b_v;
	}

	template<typename ItV, typename ItB, typename ItY>
	ItY sigm(const ItV &b_v, const ItB &b_b, const ItY &b_y) const noexcept {
		ItY it_y = b_y;
		ItB it_b = b_b;
		ItV it_v = b_v;

		for (Len i = 0; i != units_num_; ++i) {
			//DLOG(INFO) << "y[" << i << "] = sigm(" << -*it_b << " + " << *it_v << ")";
			*it_y++ = static_cast<const NeuronCalc<Float>&>(*this).sigm(*it_v++, *it_b++);
		}

		return b_y;
	}
};

} } /* namespace ::meave::ctrnn */

#endif // MEAVE_CTRNN_NEURON_HPP
