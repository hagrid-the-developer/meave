#ifndef MEAVE_GA_SIMPLE_TRIAL_HPP
#	define MEAVE_GA_SIMPLE_TRIAL_HPP

#	include "meave/lib/commons.hpp"
#	include "meave/lib/math.hpp"
#	include "meave/lib/seed.hpp"
#	include "meave/lib/str_printf.hpp"
#	include "meave/lib/xrange.hpp"
#	include "meave/ctrnn/neuron.hpp"

#	include <algorithm>
#	include <fstream>
#	include <sstream>
#	include <tuple>

namespace meave { namespace ga {

struct Nothing {
	void operator()(...) const noexcept { }
};

/**
 * @tparam Params
 *           -- Evolution Parameters --
 *           Param::gens() -- number of generaitions (10_000)
 *           Param::gaus_vec_mut -- Gassian vector mutation (0.01)
 *           Param::psize -- population size (20)
 *           Param::recprob() -- probability of recombination (0.5)
 *           Param::demewidth() -- deme width
 *
 *           -- Fitness Evaluation Parameters --
 *           Param::trial() -- units of time for each trial (50)
 *           Param::eval() -- time at which the agent starts beaing evaluated
 *           Param::repeat() -- number of trials per fitness evaluation (100)
 *           Param::velrange() -- range of different possible velocities [0, 2]
 *           Param::startposrange() -- range of different starting positions [0, 100]
 *
 *           -- euler integration parameters -- 
 *           Param::ts() -- time step of simulation <floating point>
 *
 *           -- CTRNN parameters --
 *           Param::nn() -- number of neurons (3)
 *           Param::psize -- population size
 *           Param::range() -- range of weights: [-5.0, +5.0]
 *
 *           Param::subgen_lens() -- range of lens for subgen() [+5, +5]
 */
template <typename Params>
class SimpleTrialSubGen : public Params {
public:
	typedef float Float;
	typedef uns Len;
	typedef $::default_random_engine RandomGenerator;

private:
	typedef Params P;

	class Phenotype {
	private:
		$::vector<Float> weights_;
		$::vector<Float> biases_;
		$::vector<Float> time_constants_;

	public:
		$::vector<Float> &weights() {
			return weights_;
		}
		const $::vector<Float> &weights() const {
			return weights_;
		}
		Float &weights(const ::size_t i) {
			return weights_[i];
		}
		Float weights(const ::size_t i) const {
			return weights_[i];
		}

		$::vector<Float> &biases() {
			return biases_;
		}
		const $::vector<Float> &biases() const {
			return biases_;
		}
		Float &biases(const ::size_t i) {
			return biases_[i];
		}
		Float biases(const ::size_t i) const {
			return biases_[i];
		}

		$::vector<Float> &time_constants() {
			return time_constants_;
		}
		const $::vector<Float> &time_constants() const {
			return time_constants_;
		}
		Float &time_constants(const ::size_t i) {
			return time_constants_[i];
		}
		Float time_constants(const ::size_t i) const {
			return time_constants_[i];
		}

	};

private:
	typedef meave::ctrnn::NNCalc<Float, Len> NNCalc;

	NNCalc nncalc_;
	$::vector<Float> population_;

	mutable RandomGenerator rand_;
	mutable $::uniform_real_distribution<Float> dist_;
	mutable $::normal_distribution<Float> norm_dist_;

	/**
	 * @return Genome size.
	 */
	constexpr uns gsize() const noexcept {
		return P::nn()*P::nn() + 2*P::nn();
	}

	/**
	 * @todo int/float (0.1) has big rounding error...
	 * @return Number of trials.
	 */
	constexpr uns trials_num() const {
		return static_cast<uns>( static_cast<Float>(this->trial())/this->ts() );
	}

	/**
	 * @todo int/float (0.1) has big rounding error...
	 * @return Number of timesteps after which agents starts to be evaluated.
	 */
	constexpr uns evals_num() const {
		return static_cast<uns>( static_cast<Float>(this->eval()/this->ts()) );
	}

	/**
	 * Convert index-th genotype to phenotype.
	 * @return Desired phenotype.
	 */
	Phenotype phenotype(const uns index) const {
		const Float *it_gen = &population_[index * gsize()];

		Phenotype $$;

		const Float *it_w_end = it_gen;
		$::advance(it_w_end, P::nn() * P::nn());
		$::transform(it_gen, it_w_end, $::inserter($$.weights(), $$.weights().begin()), [this](const Float $) -> Float {
			return $ * 2 * P::range() - P::range();
		});

		const Float *it_b_end = it_w_end;
		$::advance(it_b_end, P::nn());
		$::transform(it_w_end, it_b_end, $::inserter($$.biases(), $$.biases().begin()), [this](const Float $) -> Float {
			return $ * 2 * P::range() - P::range();
		});

		const Float *it_tc_end = it_b_end;
		$::advance(it_tc_end, P::nn());
		$::transform(it_b_end, it_tc_end, $::inserter($$.time_constants(), $$.time_constants().begin()), [](const Float $) -> Float {
			return ::exp(4*$);
		});

		return $$;
	}

private:
	void save_member(const uns index, const std::string &file_name) const {
		$::ofstream f(file_name);
		f.write(&population_[index*gsize()], gsize()*sizeof(population_[0]));
		f.close();
	}

	/**
	 * Fitness evaluation.
	 */
	enum FitnessKind {
		  FITNESS_RAND
		, FITNESS_FULL
	};
	template<FitnessKind FK = FITNESS_RAND, typename Wr = Nothing>
	Float fitness(const uns index, Wr wr = Nothing()) const noexcept {
		const Phenotype phe = phenotype(index);

		Float f = 0;
		if (FK == FITNESS_RAND) {
			// Regular fitness evaluation
			for (uns repeat = P::repeat(); repeat--; ) {
				const Float vel = dist_(rand_) * P::velrange();
				const Float start_pos = dist_(rand_) * P::startposrange();

				f += run_sim(start_pos, vel, phe, wr);
			}
			f /= P::repeat();
		}
		if (FK == FITNESS_FULL) {
			const uns i_max = 200;
			for (const uns i: meave::make_xrange(0U, i_max)) {
				Float temp = 0;
				const uns j_max = 11;
				for (const uns j: meave::make_xrange(0U, j_max)) {
					const uns start_pos = j*10;
					const Float f_start_pos = static_cast<Float>(start_pos);
					const Float f_vel = static_cast<Float>(i) / 100;
					const Float err = run_sim(f_start_pos, f_vel, phe, wr);
					temp += err;
				};
				temp /= j_max;
				f += temp;
			};
			f /= i_max;
		}
		return f;
	}

	/**
	 * Runs simulation for one phenotype...
	 */
	template<typename WRITER>
	Float run_sim(const double start, const double vel, const Phenotype &phenotype, WRITER wr = Nothing()) const noexcept {
		$::vector<Float> y(P::nn());
		$::vector<Float> ei(P::nn(), 0.0);
		std::vector<Float> v(P::nn());

		$::transform(phenotype.biases().begin(), phenotype.biases().end(), v.begin(), [this](const Float $) -> Float {
			return -$ + dist_(rand_) * 2 * P::range() - P::range();
		});

		Float distance = start;
		Float f = 0;
		for (uns trial_idx = 0; trial_idx < trials_num(); ++trial_idx) {
			nncalc_.sigm(v.begin(), phenotype.biases().begin(), y.begin());

			distance += P::ts() * vel;
			const Float input = distance / 20;
			ei[0] = input;
			nncalc_.val(y.begin(), phenotype.time_constants().begin(), ei.begin(), phenotype.weights().begin(), v.begin());
			const auto out = v[P::nn() - 1];

			wr(start, input, trial_idx, out, vel, f);

			if (trial_idx > evals_num()) {
				f += ::meave::math::abs(out - vel);
			}
		}

		const Float $$ = 1 - f / (P::trial() - P::eval());
		return $$;
	}

	class PairDesc {
	private:
		uns max_[2];
		uns min_;

	public:
		PairDesc(const uns max_0, const uns max_1, const uns min)
		:	max_{max_0, max_1}
		,	min_(min) {
		}
		PairDesc(const PairDesc&) = default;
		PairDesc(PairDesc&&) = default;
		~PairDesc() = default;

		uns max(const uns idx) const noexcept {
			return max_[idx];
		}

		uns min() const noexcept {
			return min_;
		}
	};

	PairDesc choose_pair() const noexcept {
		const auto subgen_lens = Params::subgen_lens();
		const auto subgen_len = $::uniform_int_distribution<uns>{$::get<0>(subgen_lens), $::get<1>(subgen_lens)}(rand_);

		$::uniform_int_distribution<uns> dist{0, P::psize() - 1};
		uns members[ subgen_len ];
		for (uns i = 0; i < subgen_len; ) {
			members[i] = dist(rand_);
			if (i == 0 || &members[i] == $::find(&members[0], &members[i], members[i]))
				++i;
		}

		Float evals[ subgen_len ];
		for(uns i = 0; i < subgen_len; ++i) {
			evals[i] = fitness<>(members[i]);
		}

		DLOG(INFO) << "choose_pair, items: [" << dbg_items(subgen_len, members, evals) << "]";
		uns max_idx[2]{0, 1};
		if (evals[0] < evals[1])
			$::swap(max_idx[0], max_idx[1]);
		uns min_idx = max_idx[1];
		for (uns i = 2; i < subgen_len; ++i) {
			if (evals[ max_idx[0] ] < evals[i]) {
				max_idx[1] = max_idx[0];
				max_idx[0] = i;
			} else if( evals[ max_idx[1] ] < evals[i]) {
				max_idx[1] = i;
			} else if (evals[min_idx] > evals[i]) {
				min_idx = i;
			}
		}

		return PairDesc(members[max_idx[0]], members[max_idx[1]], members[min_idx]);
	}
	friend $::ostream &operator<<($::ostream &_, const PairDesc &$) noexcept {
		return _ << "{" << $.max(0)
			 << ", " << $.max(1)
			 << " (" << $.min() << ")}";
	}

	template<typename It, typename Jt>
	$::string dbg_items(const uns num, const It members, const Jt fitnesses) const noexcept {
		assert(members != It());
		$::stringstream o;

		for (auto _ = 0; _ < num; _++) {
			if (_)
				o << ", ";
			o << members[_];
			if (fitnesses != Jt())
				o << "(" << fitnesses[_] << ")";
		}

		return o.str();
	}

	/**
	 * Performs transfusion and mutation on reals with a Gaussian vector mutation
	 *
	 * Note: Father is replaced by its child. :-)
	 */
	void gen_child(const PairDesc &picked) noexcept {
		static thread_local $::normal_distribution<Float> normal_distribution(0, P::gaus_vec_mut());

		const Float *m = &population_[gsize() * picked.max(0)];
		const Float *f = &population_[gsize() * picked.max(1)];
		Float *d = &population_[gsize() * picked.min()];


		for (uns i = 0; i < gsize(); ++i) {
			if (dist_(rand_) < P::recprob())
				d[i] = m[i];
			else
				d[i] = f[i];

			d[i] += norm_dist_(rand_) * P::gaus_vec_mut();
		
			if (d[i] > 1.0)
				d[i] = 2.0 - d[i];

			d[i] = ::meave::math::abs(d[i]);
		}
	}

	/**
	 * Perform's Inman's microbial algorithm with fitness eveluations anytime.
	 * Fitness function is in the range (-inf, +1] .
	 */
	void evolve() noexcept {
		$::ofstream out_worstbest("./worstbest.csv", $::ofstream::trunc);
		out_worstbest << "MinFitnessIndex" << "\t" << "MinFitnessValue" << "\t" << "MaxFitnessIndex" << "\t" << "MaxFitnessValue" << $::endl;

		for (uns popgen_idx = 0; popgen_idx < 5 * P::psize() * gsize() + 1; ++popgen_idx) {
			PairDesc picked = choose_pair();
			DLOG(INFO) << "Picked-members: " << picked;

			gen_child(picked);

			if (0 == (popgen_idx + 1) % P::psize()) {
				const uns population_idx = popgen_idx/P::psize();
				Float min_fits = +$::numeric_limits<Float>::max(); // The worst member
				Float max_fits = -$::numeric_limits<Float>::max(); // The best member
				uns min_idx = 0;
				uns max_idx = 0;
				for (uns i = 0; i < P::psize(); ++i) {
					$::ofstream out_res(str_printf("./results_%.3u_%.3u.csv", population_idx, i), $::ofstream::trunc);
					out_res << "Population" << "\t"
						<< "Member" << "\t"
						<< "Experiment" << "\t"
						<< "Start" << "\t"
						<< "Input" << "\t"
						<< "RealOutput" << "\t"
						<< "ExpectedOutput" << "\t"
						<< "f" << $::endl;
					const Float fit = fitness<FITNESS_FULL>(i, [population_idx, i, &out_res](const float start, const float input, const uns idx, const double real, const double expected, const double f) {
						out_res << population_idx << "\t"
							<< i << "\t"
							<< idx << "\t"
							<< start << "\t"
							<< input << "\t"
							<< real << "\t"
							<< expected << "\t"
							<< f << $::endl;
					});

					if (fit < min_fits)
						$::tie(min_fits, min_idx) = $::make_tuple(fit, i);
					if (fit > max_fits)
						$::tie(max_fits, max_idx) = $::make_tuple(fit, i);
				}

				out_worstbest << min_idx << "\t" << min_fits << "\t" << max_idx << "\t" << max_fits << $::endl;

				LOG(INFO) << "Population Statistics";
				LOG(INFO) << "\tPopulation: " << (popgen_idx / P::psize());
				LOG(INFO) << "\tmin-fitness (worst memmber): " << min_fits << '[' << min_idx << ']';
				LOG(INFO) << "\tmax-fitness (best member): " << max_fits << '[' << max_idx << ']';
			}
		}
	}

public:
	SimpleTrialSubGen() noexcept
	:	nncalc_(P::nn(), P::ts())
	,	population_(P::psize() * gsize())
	,	rand_(meave::seed())
	,	dist_(0.0, 1.0) {
		$::generate(population_.begin(), population_.end(), [this]() -> Float { return dist_(rand_); });
	}

	void operator()() noexcept {
		evolve();
	}
};

} } /* meave::ga */

#endif // MEAVE_GA_SIMPLE_TRIAL_HPP
