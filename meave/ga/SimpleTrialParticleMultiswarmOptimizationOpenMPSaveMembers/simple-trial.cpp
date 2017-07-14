#include <tuple>

//#include "simple-trial.hpp"
#include "oscillator-trial-001.hpp"

namespace {

struct Params {
	static constexpr uns gens() noexcept {
		return 10000;
	}

	static constexpr float gaus_vec_mut() noexcept {
		return 0.01;
	}

	static constexpr uns psize() noexcept {
		return 6*nn() + 2;
	}

	static constexpr float recprob() noexcept {
		return 1/2.f;
	}

	static constexpr uns demewidth() noexcept {
		return 5;
	}


	static constexpr uns trial() noexcept {
		return 50;
	}

	static constexpr uns eval() noexcept {
		return 30;
	}

	static constexpr uns repeat() noexcept {
		return 100;
	}

	static constexpr uns velrange() noexcept {
		return 2;
	}

	static constexpr uns startposrange() noexcept {
		return 100;
	}

	static constexpr float ts() noexcept {
		return 0.1;
	}

	static constexpr uns nn() noexcept {
		return 3;
	}

	static constexpr float range() noexcept {
		return 5.f;
	}

	static constexpr float randinit() noexcept {
		return 1.f;
	}

	struct ParticleSwarmOptimization {
		static constexpr float omega() noexcept {
			return 1/8.f;
		}

		struct {
			static constexpr float particle_best() noexcept {
				return 1/3.f;
			}
			static constexpr float subswarm_best() noexcept {
				return 1/3.f;
			}
			static constexpr float global_best() noexcept {
				return 1/3.f;
			}
		} psi;

		static constexpr uns subswarms_num() noexcept {
			return 5;
		}
	};

	static constexpr ParticleSwarmOptimization pso() noexcept {
		return ParticleSwarmOptimization();
	}
};

} /* Anonymouse Namespace */

int
main(int /* argc */, char *argv[]) {
		// Initialize Google's logging library.
		google::InitGoogleLogging(argv[0]);

		LOG(INFO) << "Dia dhuit ar domhan!";
		meave::ga::SimpleTrialParticleMultiswarmOptimization<meave::ga::SinglePrecision, Params>{}();

		return 0;
}
