#ifndef MEAVE_GA_SIMPLE_TRIAL_DIFFERENTIAL_EVOLUTION_HPP
#	define MEAVE_GA_SIMPLE_TRIAL_DIFFERENTIAL_EVOLUTION_HPP

#	include "meave/commons.hpp"
#	include "meave/lib/math.hpp"
#	include "meave/lib/seed.hpp"
#	include "meave/lib/str_printf.hpp"
#	include "meave/lib/xrange.hpp"
#	include "meave/ctrnn/neuron.hpp"

#	include <algorithm>
#	include <future>
#	include <fstream>
#	include <sstream>
#	include <random>
#	include <tuple>

namespace meave { namespace ga {

struct Nothing {
	void operator()(...) const noexcept { }
};

struct SinglePrecision {
	typedef float Float;
	typedef uns Len;
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
 *           Param::eval() -- time at which the agent starts being evaluated
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
template <typename Types, typename Params>
class SimpleTrialParticleSwarmOptimization : public Types
				       , public Params {
public:
	typedef typename Types::Float Float;
	typedef typename Types::Len Len;
	typedef std::default_random_engine RandomGenerator;

	Float dist() noexcept {
		return dist_(rand_);
	}

	Float norm_dist() noexcept {
		return norm_dist_(rand_);
	}

	template<int a, int b, int div>
	Float uniform_dist() noexcept {
		static thread_local $::uniform_real_distribution<Float> dist(a/float(div), b/float(div));
		return dist(rand_);
	}

	template<int mean, int stddev, int div>
	Float normal_dist() noexcept {
		static thread_local $::normal_distribution<Float> dist(mean/float(div), stddev/float(div));
		return dist(rand_);
	}

	template<uns a, uns b>
	uns uniform_uns_dist() noexcept {
		static thread_local $::uniform_int_distribution<uns> dist(a, b);
		return dist(rand_);
	}

	uns rand_index() noexcept {
		static thread_local $::uniform_int_distribution<uns> dist(0, P::psize() - 1);
		return dist(rand_);
	}

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
	$::vector<Float> positions_;
	$::vector<Float> best_positions_;
	$::vector<Float> velocities_;
	$::vector<Float> best_fitnesses_;

	mutable RandomGenerator rand_;
	mutable $::uniform_real_distribution<Float> dist_;
	mutable $::normal_distribution<Float> norm_dist_;

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
		const Float *it_gen = &positions_[index * gsize()];

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
		f.write(&positions_[index*gsize()], gsize()*sizeof(positions_[0]));
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
	Float fitness(const uns index, Wr wr = Nothing()) noexcept {
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

	void maybe_gen_child(const uns picked_idx) noexcept {
		// https://en.wikipedia.org/wiki/Particle_swarm_optimization
		DLOG(INFO) << "Picked idx:" << picked_idx;

		Float *x = &positions_[gsize() * picked_idx];
		Float *best_x = &best_positions_[gsize() * picked_idx];
		Float *global_best_x = &best_positions_[gsize() * P::psize()];

		Float *velocity = &velocities_[gsize() * picked_idx];

		const uns pso_p = rand_index();
		const uns pso_g = rand_index();
		const Float rp = uniform_dist<0, 1, 1>();
		const Float rg = uniform_dist<0, 1, 1>();
		Float *xp = &positions_[gsize() * pso_p];
		Float *xg = &positions_[gsize() * pso_g];

		for (uns _ = 0; _ < gsize(); ++_) {
			DLOG(INFO) << ">> " << P::pso().omega() << " * " << velocity[_] << " + " << P::pso().psi[pso_p] << " * " << rp << " * " << (xp[_] - x[_]) << " + " << P::pso().psi[pso_g] << " * " << rg << " * " << (xg[_] - x[_]);
			velocity[_] = P::pso().omega() * velocity[_] + P::pso().psi[pso_p] * rp * (xp[_] - x[_]) +  P::pso().psi[pso_g] * rg * (xg[_] - x[_]);
		}

		for (uns _ = 0; _ < gsize(); ++_) {
			x[_] += velocity[_];
		}

		const Float fit = fitness<FITNESS_FULL>(picked_idx);
		DLOG(INFO) << "New member generated:\n"
			      "\tmin - max = " << *$::min_element(&x[0], &x[gsize()]) << " - " << *$::max_element(&x[0], &x[gsize()]) << "\n"
			      "\tvmin - vmax = " << *$::min_element(&velocity[0], &velocity[gsize()]) << " - " << *$::max_element(&velocity[0], &velocity[gsize()]) << "\n"
			      "\tnew fitness, previous fitness, global fitness: " << fit << "; " << best_fitnesses_[picked_idx] << "; " << best_fitnesses_[P::psize()];
		if (best_fitnesses_[picked_idx] < fit) {
			DLOG(INFO) << "Rewriting local best";
			$::copy(&x[0], &x[gsize()], best_x);
			best_fitnesses_[picked_idx] = fit;
		}
		if (best_fitnesses_[P::psize()] < fit) {
			DLOG(INFO) << "Rewriting global best";
			$::copy(&x[0], &x[gsize()], global_best_x);
			best_fitnesses_[P::psize()] = fit;
		}
	}

	template<typename It, typename Jt>
	$::string dbg_items(const uns num, const It members, const Jt fitnesses) const noexcept {
		assert(members != It());
		$::stringstream o;

		for (uns _ = 0; _ < num; _++) {
			if (_)
				o << ", ";
			o << members[_];
			if (fitnesses != Jt())
				o << "(" << fitnesses[_] << ")";
		}

		return o.str();
	}

	/**
	 * Perform's Differential evolution.
	 * Fitness function is in the range (-inf, +1] .
	 */
	void evolve() noexcept {
		$::ofstream out_worstbest("./worstbest.csv", $::ofstream::trunc);
		out_worstbest << "MinFitnessIndex" << "\t" << "MinFitnessValue" << "\t" << "MaxFitnessIndex" << "\t" << "MaxFitnessValue" << $::endl;

		for (uns popgen_idx = 0; popgen_idx < 5 * P::psize() * gsize() + 1; ++popgen_idx) {
			maybe_gen_child(popgen_idx % P::psize());

			if (0 == (popgen_idx + 1) % P::psize()) {
				const uns positions_idx = popgen_idx/P::psize();
				Float min_fits = +$::numeric_limits<Float>::max(); // The worst member
				Float max_fits = -$::numeric_limits<Float>::max(); // The best member
				uns min_idx = 0;
				uns max_idx = 0;
				for (uns i = 0; i < P::psize(); ++i) {
					$::ofstream out_res(str_printf("./results_%.3u_%.3u.csv", positions_idx, i), $::ofstream::trunc);
					out_res << "Population" << "\t"
						<< "Member" << "\t"
						<< "Experiment" << "\t"
						<< "Start" << "\t"
						<< "Input" << "\t"
						<< "RealOutput" << "\t"
						<< "ExpectedOutput" << "\t"
						<< "f" << "\n";
					const Float fit = fitness<FITNESS_FULL>(i, [positions_idx, i, &out_res](const float start, const float input, const uns idx, const double real, const double expected, const double f) {
						out_res << positions_idx << "\t"
							<< i << "\t"
							<< idx << "\t"
							<< start << "\t"
							<< input << "\t"
							<< real << "\t"
							<< expected << "\t"
							<< f << "\n";
					});

					if (fit < min_fits)
						$::tie(min_fits, min_idx) = $::make_tuple(fit, i);
					if (fit > max_fits)
						$::tie(max_fits, max_idx) = $::make_tuple(fit, i);
				}

				out_worstbest << min_idx << "\t" << min_fits << "\t" << max_idx << "\t" << max_fits << "\n";

				LOG(INFO) << "Population Statistics";
				LOG(INFO) << "\tPopulation: " << (popgen_idx / P::psize());
				LOG(INFO) << "\tmin-fitness (worst memmber): " << min_fits << '[' << min_idx << ']';
				LOG(INFO) << "\tmax-fitness (best member): " << max_fits << '[' << max_idx << ']';
			}
		}
	}

public:
	SimpleTrialParticleSwarmOptimization() noexcept
	:	nncalc_(P::nn(), P::ts())
	,	positions_(P::psize() * gsize())
	,	best_positions_((P::psize() + 1) * gsize())
	,	velocities_(P::psize() * gsize())
	,	best_fitnesses_(P::psize() + 1)
	,	rand_(meave::seed()) {
		$::generate(positions_.begin(), positions_.end(), [this]() -> Float { return uniform_dist<0, +1, 1>(); });
		$::generate(velocities_.begin(), velocities_.end(), [this]() -> Float { return uniform_dist<-1, +1, 1>(); });
		$::copy(positions_.begin(), positions_.end(), best_positions_.begin());
		for (uns _ = 0; _ < P::psize(); ++_) {
			best_fitnesses_[_] = fitness<FITNESS_FULL>(_);
		}
		// .end() - 1, because last element is used for the best fitness of the whole population.
		const auto max_idx = $::max_element(best_fitnesses_.begin(), best_fitnesses_.end() - 1) - best_fitnesses_.begin();
		$::copy(&positions_[max_idx * gsize()], &positions_[(max_idx + 1)*gsize()], &best_positions_[P::psize() * gsize()]);
		best_fitnesses_[P::psize()] = best_fitnesses_[max_idx];
	}

	void operator()() noexcept {
		evolve();
	}
};

} } /* meave::ga */

#endif // MEAVE_GA_SIMPLE_TRIAL_DIFFERENTIAL_EVOLUTION_HPP
