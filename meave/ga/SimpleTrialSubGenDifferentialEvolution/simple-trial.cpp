#include <tuple>

#include "simple-trial.hpp"

namespace {

struct Params {
	constexpr uns gens() const noexcept {
		return 10000;
	}

	constexpr float gaus_vec_mut() const noexcept {
		return 0.01;
	}

	constexpr uns psize() const noexcept {
		return 6*nn() + 2;
	}

	constexpr float recprob() const noexcept {
		return 1/2.f;
	}

 	constexpr uns demewidth() const noexcept {
		return 5;
	}

 
 	constexpr uns trial() const noexcept {
		return 50;
	}

	constexpr uns eval() const noexcept {
		return 30;
	}

	constexpr uns repeat() const noexcept {
		return 100;
	}

 	constexpr uns velrange() const noexcept {
		return 2;
	}

	constexpr uns startposrange() const noexcept {
		return 100;
	}

	constexpr float ts() const noexcept {
		return 0.1;
	}

 	constexpr uns nn() const noexcept {
		return 3;
	}

	constexpr float range() const noexcept {
		return 5.f;
	}

	constexpr $::tuple<uns, uns> subgen_lens() const noexcept {
		return $::make_tuple(5, 5);
	}

	struct DifferentialEvolution {
		constexpr float differential_weight() const noexcept {
			return 1/8.f;
		}

		constexpr float crossover_probability() const noexcept {
			return 1/2.f;
		}
	};

	constexpr DifferentialEvolution differential_evolution() const noexcept {
		return DifferentialEvolution();
	}
};

} /* Anonymouse Namespace */

int
main(int argc, char *argv[]) {
		// Initialize Google's logging library.
		google::InitGoogleLogging(argv[0]);

		meave::ga::SimpleTrialDifferentialEvolution<meave::ga::SinglePrecision, Params>{}();

		return 0;
}
