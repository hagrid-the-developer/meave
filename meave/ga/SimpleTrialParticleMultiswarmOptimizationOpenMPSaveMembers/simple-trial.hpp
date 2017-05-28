#ifndef MEAVE_GA_SIMPLE_TRIAL_PARTICLE_MULTISWARM_OPTIMIZATION_HPP

#	define MEAVE_GA_SIMPLE_TRIAL_PARTICLE_MULTISWARM_OPTIMIZATION_HPP

#	include <meave/commons.hpp>
#	include <meave/ctrnn/neuron.hpp>
#	include <meave/lib/math.hpp>
#	include <meave/lib/raii/accumulate_flush.hpp>
#	include <meave/lib/raii/mmap_create.hpp>
#	include <meave/lib/seed.hpp>
#	include <meave/lib/str_printf.hpp>
#	include <meave/lib/xrange.hpp>

#	include <algorithm>
#	include <atomic>
#	include <numeric>
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
class SimpleTrialParticleMultiswarmOptimization : public Types
						, public Params {
public:
	typedef typename Types::Float Float;
	typedef typename Types::Len Len;

	class RandomGenerator : public $::default_random_engine {
	public:
		RandomGenerator()
		:	$::default_random_engine(meave::seed()) {
		}
	};

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
	static constexpr uns gsize() noexcept {
		return P::nn()*P::nn() + 2*P::nn();
	}

	/**
	 * @todo int/float (0.1) has big rounding error...
	 * @return Number of trials.
	 */
	constexpr uns trials_num() const {
		return static_cast<uns>( static_cast<Float>(this->trial())/this->ts() );
	}

	constexpr uns cpus_num() const noexcept {
		return 8;
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
	$::vector<Float> best_subswarm_positions_;
	$::vector<Float> best_subswarm_fitnesses_;

	$::vector<uns> subswarm_map_;

	const uns cpus_num_;

	static thread_local RandomGenerator rand_;
	static thread_local $::uniform_real_distribution<Float> dist_;
	static thread_local $::normal_distribution<Float> norm_dist_;
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

		if (FK == FITNESS_RAND) {
			Float sum = 0;
			#pragma omp parallel for reduction(+:sum)
			for (uns _ = 0; _ < P::repeat(); ++_) {
				const Float vel = dist_(rand_) * P::velrange();
				const Float start_pos = dist_(rand_) * P::startposrange();

				sum += run_sim(start_pos, vel, phe, wr);
			}
			return sum / P::repeat();
		}
		if (FK == FITNESS_FULL) {
			const uns i_max = 200;
			Float sum = 0;
			#pragma omp parallel for reduction(+:sum)
			for (uns _ = 0; _ < i_max; ++_) {
				Float temp = 0;
				const uns j_max = 11;
				for (const uns j: meave::make_xrange(0U, j_max)) {
					const uns start_pos = j*10;
					const Float f_start_pos = Float(start_pos);
					const Float f_vel = Float(_) / 100;
					const Float err = run_sim(f_start_pos, f_vel, phe, wr);
					temp += err;
				}
				sum += temp /= j_max;
			}
			return sum / i_max;
		}
		assert(0);
		return Float();
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
			return -$ + dist_(rand_) * 2 * P::randinit() - P::randinit();
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

		Float *v = &velocities_[gsize() * picked_idx];

		const uns subswarm_idx = subswarm_map_[picked_idx];
		Float *subswarm_x = &best_subswarm_positions_[gsize() * subswarm_idx];

		for (uns _ = 0; _ < gsize(); ++_) {
			const Float rp = uniform_dist<0, 1, 1>();
			const Float rs = uniform_dist<0, 1, 1>();
			const Float rg = uniform_dist<0, 1, 1>();

			DLOG(INFO) << "v[" << _ << "] = " << P::pso().omega() << " * " << v[_] << " + "
				   << P::pso().psi.particle_best() << " * " << rp << " * " << "(" << best_x[_] << " - " << x[_] << ")" << " + "
				   << P::pso().psi.subswarm_best() << " * " << rs << " * " << "(" << subswarm_x[_] << " - " << x[_] << ")" << " + "
				   << P::pso().psi.global_best() << " * " << rg << " * " << "(" << global_best_x[_] << " - " << x[_] << ")";
			v[_] = P::pso().omega() * v[_] +
				      P::pso().psi.particle_best() * rp * (best_x[_] - x[_]) +
				      P::pso().psi.subswarm_best() * rs * (subswarm_x[_] - x[_]) +
				      P::pso().psi.global_best() * rg * (global_best_x[_] - x[_]);
		}

		for (uns _ = 0; _ < gsize(); ++_) {
			DLOG(INFO) << "x[" << _ << "] += " << v[_];
			x[_] += v[_];
		}

		const Float fit = fitness<FITNESS_FULL>(picked_idx);
		DLOG(INFO) << "New member generated:\n"
			      "\tmin - max = " << *$::min_element(&x[0], &x[gsize()]) << " - " << *$::max_element(&x[0], &x[gsize()]) << "\n"
			      "\tvmin - vmax = " << *$::min_element(&v[0], &v[gsize()]) << " - " << *$::max_element(&v[0], &v[gsize()]) << "\n"
			      "\tnew fitness, previous fitness, global fitness: " << fit << "; " << best_fitnesses_[picked_idx] << "; " << best_fitnesses_[P::psize()];
		if (best_fitnesses_[picked_idx] < fit) {
			DLOG(INFO) << "Rewriting local best";
			$::copy(&x[0], &x[gsize()], best_x);
			best_fitnesses_[picked_idx] = fit;
		}
		if (best_subswarm_fitnesses_[subswarm_idx] < fit) {
			DLOG(INFO) << "Rewriting subswarm best";
			$::copy(&x[0], &x[gsize()], subswarm_x);
			best_subswarm_fitnesses_[subswarm_idx] = fit;
		}
		if (best_fitnesses_[P::psize()] < fit) {
			DLOG(INFO) << "Rewriting global best";
			$::copy(&x[0], &x[gsize()], global_best_x);
			best_fitnesses_[P::psize()] = fit;
		}
	}

	struct Result {
		uns population_idx_;
		uns i_;
		uns idx_;
		float start_;
		float input_;
		double real_;
		double expected_;
		double f_;

		Result() = default;
		Result(const uns population_idx,
			uns i,
			uns idx,
			float start,
			float input,
			double real,
			double expected,
			double f) noexcept
		:	population_idx_(population_idx)
		,	i_(i)
		,	idx_(idx)
		,	start_(start)
		,	input_(input)
		,	real_(real)
		,	expected_(expected)
		,	f_(f) {
		}
		Result(const Result&) = default;
		Result(Result&&) = default;

		~Result() = default;
	};

	struct PopulationMinMax {
		uns min_pos_;
		uns max_pos_;
		Float min_fits_;
		Float max_fits_;

		PopulationMinMax() noexcept
		:	min_pos_(0)
		,	max_pos_(0)
		,	min_fits_($::numeric_limits<Float>::max())
		,	max_fits_(-$::numeric_limits<Float>::max()) {
		}
		PopulationMinMax(const uns min_pos, const uns max_pos, const Float min_fits, const Float max_fits)
		:	min_pos_(min_pos)
		,	max_pos_(max_pos)
		,	min_fits_(min_fits)
		,	max_fits_(max_fits) {
		}

		friend PopulationMinMax operator+(const PopulationMinMax &a, const PopulationMinMax &b) {
			PopulationMinMax $$;

			if (a.min_fits_ <= b.min_fits_) {
				$$.min_pos_ = a.min_pos_;
				$$.min_fits_ = a.min_fits_;
			} else {
				$$.min_pos_ = b.min_pos_;
				$$.min_fits_ = b.min_fits_;
			}

			if (a.max_fits_ >= b.max_fits_) {
				$$.max_pos_ = a.max_pos_;
				$$.max_fits_ = a.max_fits_;
			} else {
				$$.max_pos_ = b.max_pos_;
				$$.max_fits_ = b.max_fits_;
			}

			return $$;
		}

		PopulationMinMax &operator+=(const PopulationMinMax &$) {
			if (min_fits_ >= $.min_fits_) {
				min_pos_ = $.min_pos_;
				min_fits_ = $.min_fits_;
			}

			if (max_fits_ <= $.max_fits_) {
				max_pos_ = $.max_pos_;
				max_fits_ = $.max_fits_;
			}

			return *this;
		}
	};

	/**
	 * Perform's Particle MultiSwarm Optimization.
	 * Fitness function is in the range (-inf, +1] .
	 */
	void evolve() {
		struct X {
			$::string name_;

			X() {
				$::stringstream ss;
				ss << $::this_thread::get_id();
				name_ = ss.str();
			}

			~X() noexcept {
				LOG(INFO) << "Destructing: " << name_ << ';';
			}
		};
		struct MemberPod {
			uns population_id_;
			uns member_id_;
			Float genome_[gsize()];

			MemberPod() = delete;
			MemberPod(const uns population_id, const uns member_id, const Float *g)
			:	population_id_(population_id)
			,	member_id_(member_id) {
				::memcpy(genome_, g, sizeof genome_);
			}
		};

		raii::MMapCreate members_mem("members.dat", (5 * P::psize() * gsize() + 1) * sizeof(MemberPod));
		union {
			void *v_;
			MemberPod *p_;
		} members{*members_mem};
		$::ofstream out_worstbest("./worstbest.csv", $::ofstream::trunc);
		out_worstbest << "MinFitnessIndex" << "\t" << "MinFitnessValue" << "\t" << "MaxFitnessIndex" << "\t" << "MaxFitnessValue" << $::endl;

		static thread_local std::unique_ptr<X> hokus;
		for (uns popgen_idx = 0; popgen_idx < 5 * P::psize() * gsize() + 1; ++popgen_idx) {
			maybe_gen_child(popgen_idx % P::psize());

			if (0 == (popgen_idx + 1) % P::psize()) {
				const uns positions_idx = popgen_idx/P::psize();
				PopulationMinMax pmM;
				#pragma omp declare reduction(PopulationMinMaxReduction: class PopulationMinMax: \
					omp_out=(omp_out + omp_in) ) \
					initializer( omp_priv= PopulationMinMax() )
				#pragma omp parallel for reduction(PopulationMinMaxReduction:pmM)
				for (uns _ = 0; _ < P::psize(); ++_) {
					if (!hokus) {
						hokus.reset(new X);
					}
					LOG(INFO) << "Current thread: " << $::this_thread::get_id() << "; hokus: " << static_cast<void*>(hokus.get());
					const Float fit = fitness<FITNESS_FULL>(_);

					new(&members.p_[popgen_idx - _]) MemberPod{positions_idx, _, &positions_[_*gsize()]};
					pmM += PopulationMinMax(_, _, fit, fit);
				};

				out_worstbest << pmM.min_pos_ << "\t" << pmM.min_fits_ << "\t" << pmM.max_pos_ << "\t" << pmM.max_fits_ << "\n";

				LOG(INFO) << "Population Statistics";
				LOG(INFO) << "\tPopulation: " << popgen_idx / P::psize();
				LOG(INFO) << "\tmin-fitness (worst memmber): " << pmM.min_fits_ << '[' << pmM.min_pos_ << ']';
				LOG(INFO) << "\tmax-fitness (best member): " << pmM.max_fits_ << '[' << pmM.max_pos_ << ']';
			}
		}
	}

public:
	SimpleTrialParticleMultiswarmOptimization() noexcept
	:	nncalc_(P::nn(), P::ts())
	,	positions_(P::psize() * gsize(), 0.f)
	,	best_positions_((P::psize() + 1) * gsize(), 0.f)
	,	velocities_(P::psize() * gsize(), 0.f)
	,	best_fitnesses_(P::psize() + 1, 0.f)
	,	best_subswarm_positions_(P::pso().subswarms_num() * gsize(), 0.f)
	,	best_subswarm_fitnesses_(P::pso().subswarms_num(), 0.f)
	,	subswarm_map_(P::psize(), 0)
	,	cpus_num_(cpus_num()) {
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

		for (uns i = 0; i < subswarm_map_.size(); ++i) {
			subswarm_map_[i] = i % P::pso().subswarms_num();
		}
		$::sort(subswarm_map_.begin(), subswarm_map_.end());

		for (uns _ = 0, prev_subswarm_idx = ~0U, subswarm_idx; _ < P::psize(); ++_, prev_subswarm_idx = subswarm_idx) {
			subswarm_idx = subswarm_map_[_];

			const bool copy = prev_subswarm_idx != subswarm_idx || best_subswarm_fitnesses_[subswarm_idx] < best_fitnesses_[_];

			if (copy)
				$::copy(&positions_[_ * gsize()],
					&positions_[(_ + 1) * gsize()],
					&best_subswarm_positions_[subswarm_idx * gsize()]);
		}

	}

	void operator()() {
		evolve();
	}

	~SimpleTrialParticleMultiswarmOptimization() noexcept {
	}
};

template <typename Types, typename Params>
thread_local typename SimpleTrialParticleMultiswarmOptimization<Types, Params>::RandomGenerator SimpleTrialParticleMultiswarmOptimization<Types, Params>::rand_;

template <typename Types, typename Params>
thread_local $::uniform_real_distribution<typename SimpleTrialParticleMultiswarmOptimization<Types, Params>::Float> SimpleTrialParticleMultiswarmOptimization<Types, Params>::dist_;

template <typename Types, typename Params>
thread_local $::normal_distribution<typename SimpleTrialParticleMultiswarmOptimization<Types, Params>::Float> SimpleTrialParticleMultiswarmOptimization<Types, Params>::norm_dist_;

} } /* meave::ga */

#endif // MEAVE_GA_SIMPLE_TRIAL_PARTICLE_MULTISWARM_OPTIMIZATION_HPP
