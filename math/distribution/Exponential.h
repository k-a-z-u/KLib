#ifndef K_MATH_DISTRIBUTION_EXPONENTIAL_H
#define K_MATH_DISTRIBUTION_EXPONENTIAL_H

#include <algorithm>
#include <random>

namespace K {

	class ExponentialDistribution {

	private:

		double lambda;

		std::random_device rd;
		std::mt19937 gen;
		std::exponential_distribution<> dist;

	public:

		/** ctor */
		ExponentialDistribution(const double lambda) : lambda(lambda), gen(rd()), dist(lambda) {
			;
		}


		/** get probability for the given value */
		double getProbability(const double val) const {
			return lambda * std::exp(-lambda * val);
		}

		/** get a normally distributed random number */
		double draw() {
			return dist(gen);
		}

		/** set the seed to use */
		void setSeed(const long seed) {
			gen.seed(seed);
		}


		/** get the probability for the given value */
		static double getProbability(const double lambda, const double val) {
			return lambda * std::exp(-lambda * val);
		}

	};

}

#endif // K_MATH_DISTRIBUTION_EXPONENTIAL_H
