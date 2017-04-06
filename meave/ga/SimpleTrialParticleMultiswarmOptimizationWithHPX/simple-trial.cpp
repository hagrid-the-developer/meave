#include <hpx/hpx_init.hpp>
#include <hpx/runtime/actions/plain_action.hpp>

#include <boost/program_options.hpp>

#include <tuple>

#include "meave/lib/str_printf.hpp"
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

	struct ParticleSwarmOptimization {
		constexpr float omega() const noexcept {
			return 1/8.f;
		}

		struct {
			constexpr float particle_best() const noexcept {
				return 1/3.f;
			}
			constexpr float subswarm_best() const noexcept {
				return 1/3.f;
			}
			constexpr float global_best() const noexcept {
				return 1/3.f;
			}
		} psi;

		constexpr uns subswarms_num() const noexcept {
			return 5;
		}
	};

	constexpr ParticleSwarmOptimization pso() const noexcept {
		return ParticleSwarmOptimization();
	}
};

} /* Anonymouse Namespace */

#if 0
namespace aux {
	int run(boost::program_options::variables_map&) {
	$::cerr << "xyz[" <<  __FILE__ << ':' << __LINE__ << ']' << $::endl;
		meave::ga::SimpleTrialParticleMultiswarmOptimization<meave::ga::SinglePrecision, Params>{}();
		return 0;
	}
} /* Namespace aux */
//HPX_PLAIN_ACTION(aux::run, run_particle_swarm_optimization_action);

int hpx_main(int argc, char* argv[]) {
	$::cerr << "xyz[" <<  __FILE__ << ':' << __LINE__ << ']' << $::endl;
    // Any HPX application logic goes here...
    return hpx::finalize();
}
#endif

#include <hpx/hpx_main.hpp>

int main(int argc, char* argv[]) {
	$::cerr << "xyz[" <<  __FILE__ << ':' << __LINE__ << ']' << $::endl;
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
/*
	$::cerr << "xyz[" <<  __FILE__ << ':' << __LINE__ << ']' << $::endl;
	boost::program_options::options_description desc_cmdline(meave::str_printf("usage: %s [options]", argv[0]));

	$::cerr << "xyz[" <<  __FILE__ << ':' << __LINE__ << ']' << $::endl;
	// Initialize HPX, run hpx_main as the first HPX thread, and
	// wait for hpx::finalize being called.
*/

	meave::ga::SimpleTrialParticleMultiswarmOptimization<meave::ga::SinglePrecision, Params>{}();
	return 0;

//	return hpx::init(aux::run, desc_cmdline, argc, argv);
}
