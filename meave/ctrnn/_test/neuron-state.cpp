#include <array>
#include <boost/assert.hpp>
#include <glog/logging.h>
#include <iostream>
#include <meave/commons.hpp>
#include <meave/lib/math.hpp>
#include <meave/lib/_42.hpp>
#include <meave/ctrnn/neuron.hpp>

template<typename Float>
struct Fánach {
public:
	static Float rand_float() noexcept {
		return (static_cast<Float>(rand()) / static_cast<Float>(RAND_MAX));
	}
};


class Worker : public Fánach<float> {
private:

public:
	typedef float Float;
	typedef std::array<Float, 3> Triple;
	typedef std::vector<Triple> Vec;

	Triple rand_triple() const noexcept {
		Triple triple;
		for (uns i = 0; i < 3; ++i) {
			triple[i] = rand_float();
		}
		return triple;
	}

	Vec rand_vec(const uns len) const noexcept {
		Vec vec;

		for (uns i = 0; i < len; ++i) {
			Triple triple = rand_triple();
			vec.push_back(triple);
		}

		return vec;
	}

	Float calc_real(const meave::ctrnn::NeuronCalc<Float> &neuron, const Float time_constant, const Float bias, const Float &first, const Triple &ws, const Vec& inputs) const {
		Float y = first;

		for (const auto &in: inputs) {
			y = neuron.val(y, time_constant, 0.0, &in[0], &in[3], &ws[0], &ws[3]);
		}

		return neuron.sigm(y, bias);
	}

	Float calc_expected(const meave::ctrnn::NeuronCalc<Float> &neuron, const Float time_constant, const Float bias, const Float &first, const Triple &ws, const Vec& inputs) const {
		const Float TS = neuron.time_step();
		const Float T = time_constant;
		Float V = first;


		for (const auto &in: inputs) {
			Float temp = 0.0;
			for (uns k = 0; k < 3; k++)
				temp += ws[k] * in[k];
			V = V + TS*((1/T)*(-V + temp + 0.0));
		}

		return neuron.sigm(V, bias);
	}

	int operator()() const noexcept {
		const meave::ctrnn::NeuronCalc<Float> neuron(meave::ctrnn::TimeStep<Float>(0.2));

		const Float bias = rand_float();
		const Float first = rand_float();
		const Triple ws = rand_triple();
		const Vec inputs = rand_vec(256);
		const Float time_constant = 0.1;

		const Float out_real = calc_real(neuron, time_constant, bias, first, ws, inputs);
		const Float out_expected = calc_expected(neuron, time_constant, bias, first, ws, inputs);

		const Float abs_err = meave::math::abs_err(out_real, out_expected);
		LOG(INFO) << "Absolute error between real and expected is: " << abs_err;
		CHECK(abs_err < 0.0001) << "Absolute error is too big";

		return 0;
	}
};

/*
 * Functions stolen from:
 *  http://www.sussex.ac.uk/Users/inmanh/easy/alife10/GA+CTRNN%20Exercise%20--%20Example%20Code.html
 */
class ___ : public Fánach<double> {
public:
	static const unsigned TRIAL = 50;    // units of time for each trial
	static const unsigned EVAL = 30;     // time at which the agent starts being evaluated
	static const unsigned REPEAT = 100;  // number of trials per fitness evaluation
	static const unsigned VELRANGE = 2;  // range of different possible velocities [0,2]
	static const unsigned STARTPOSRANGE = 100;  // range of different starting positions [0,100]

 //---------------- euler integration parameters
	static constexpr double TS = 0.1;        // time step of the simulation
	static const unsigned TRIALTS = (unsigned)(TRIAL/TS); // conversion to No. of timesteps
	static const unsigned EVALTS = (unsigned)(EVAL/TS);  // conversion to No. of timesteps

 //---------------- CTRNN parameters
	static constexpr unsigned NN = 3;        // number of neurons
	static constexpr unsigned getNN() noexcept { return NN; }
	static const unsigned GSIZE = (NN*NN+2*NN);      // genome size
	static const unsigned RANGE = 5;     // range of weights [-5,5]
	static const unsigned RANDINIT = 1;  // random initialization of node activations [-bias-1,-bias+1]

 //---------------- random generator parameters
	static const unsigned IA = 16807;
	static const unsigned IM = 2147483647;
	static constexpr double AM = (1.d / IM);
	static const unsigned IQ = 127773;
	static const unsigned IR = 2836;
	static const unsigned NTAB = 32;
	static const unsigned NDIV = (1+(IM-1)/NTAB);
	static constexpr double EPS = 1.2e-7;
	static constexpr double RNMX = (1.0-EPS);

	___()
	:	start_(rand_float())
	,	vel_(rand_float())
	{
		const double T = rand_float();
		for (int i = 0; i < NN; ++i) {
			V_[i] = rand_float();
			T_[i] = T;
			B_[i] = rand_float();

			for (int j = 0; j < NN; ++j) {
				W_[i][j] = rand_float();
			}
		}
	}

protected:
	//---------------- fitness evaluation parameters

	double start_;
	double vel_;
	double V_[NN];
	double T_[NN];
	double B_[NN];
	double W_[NN][NN];

	static double runSim(double start, double vel, double V[NN], const double T[NN], const double W[NN][NN], const double B[NN]) {
	       double f=0.0;
	       double eI[NN];
	       double SIGM[NN];
	       unsigned i,j,k;
	       double dist=0,temp;

		double SIGMOID[10000];                           //look-up table for sigmoid function
		for (int i=0;i<10000;i++)
			SIGMOID[i]=1/(1+exp(-((double)(i-5000)/500)));

	       dist = start;

	       // euler integration por LEN units of time
	       for (i=0; i<TRIALTS; i++) {
	              // update position of the target
	              dist+=TS*vel;

	              // update sensory input
	              eI[0]=dist/20;
		      for (unsigned idx = 1; idx < NN; ++idx)
			      eI[idx] = 0.0;

	              // calculate sigmoided outputs of the nodes
	              for   (j=0;j<NN;j++){
	                      temp=V[j]+B[j];
			    /*
			     * We cannot use precalculated sigmoid, since difference is then too big.
	                      if (temp<-10) SIGM[j]=SIGMOID[0];
	                      else if (temp>=10) SIGM[j]=SIGMOID[9999];
	                      else SIGM[j]=SIGMOID[(int)(temp*500)+5000];
			      */
				SIGM[j] = meave::math::sigmoid(temp);
	              }

	              // update the nodes in the CTRNN
	              for (j=0;j<NN;j++){
	                      temp=0;
	                      for (k=0;k<NN;k++) {
				LOG(INFO) << "Sum[" << j << "] += " << W[k][j] << " + " << SIGM[k];
	                             temp += W[k][j]*SIGM[k];
			      }
	                      V[j]=V[j]+TS*((1/T[j])*(-V[j]+temp+eI[j]));
			LOG(INFO) << "next_val[" << j << "] = " << V[j] << " + " << TS << "* (" << -V[j] << " + " << eI[j] << " + " << temp << ") / " << T[j];
	              }
		      LOG(INFO) << "v[NN - 1]:" << V[NN - 1] << "; vel:" << vel << "; f:" << f;
	              // target output
	              if (i>EVALTS)
			      f+=fabs(V[NN-1]-vel);
	       }

	       return f;
	}

public:
	double operator()() const noexcept {
		double v[NN];
		for (uns i = 0; i < NN; ++i)
			v[i] = V_[i];

		return runSim(start_, vel_, v, T_, W_, B_);
	}
};

class NetWorker : public ___ {
private:
	::meave::ctrnn::NNCalc<float, uns> nn_;

public:
	NetWorker()
	:	nn_(getNN(), TS) { }

	int operator()() const noexcept {
		const double res_expected = static_cast<const ___&>(*this)();

		float res_real = 0.0;
		float eI[NN];
		for (uns i = 0; i < NN; ++i) {
			eI[i] = 0.0;
		}
	
		double v[NN];
		for (uns i = 0; i < NN; ++i)
			v[i] = V_[i];

		double w[NN*NN];
		for (uns i = 0; i < NN; ++i) {
			for (uns j = 0; j < NN; ++j) {
				w[i*NN + j] = W_[j][i];
			}
		}

		float dist = start_;

		double y[NN];
		nn_.sigm(&v[0], &B_[0], &y[0]);

		for (uns trial_idx = 0; trial_idx < TRIALTS; ++trial_idx) {
			dist += TS * vel_;
			eI[0] = dist/20;

			nn_.val(&y[0], &T_[0], &eI[0], &w[0], &v[0]);
			nn_.sigm(&v[0], &B_[0], &y[0]);

			LOG(INFO) << "v[NN - 1]:" << v[NN - 1] << "; vel:" << vel_ << "; res-real:" << res_real;

			// target output
			if (trial_idx > EVALTS)
				res_real += fabs(v[NN-1] - vel_);
		}

		const float abs_err = meave::math::abs_err(res_real, static_cast<float>(res_expected));
		LOG(INFO) << "Absolute error between real and expected is: " << abs_err;
		CHECK(abs_err < 0.0001) << "Absolute error is too big";

		return 0;
	}
};


// @todo: Main as a template that goes through all of its args...
class Main : public ::meave::_42<Main> {
private:

public:
	using _42::_42;

	int operator()() const noexcept {
		const int status_worker = Worker{}();
		LOG(INFO) << "Worker returned status:" << status_worker;

		const int status_networker = NetWorker{}();
		LOG(INFO) << "NetWorker returned status:" << status_networker;

		return status_worker == 0 && status_networker == 0 ? 0 : 1;
	}
};

int
main(int argc, char *argv[]) {
	return Main{argc, argv}();
}
