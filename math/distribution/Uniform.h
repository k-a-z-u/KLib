#ifndef K_MATH_DISTRIBUTIONS_UNIFORM_H
#define K_MATH_DISTRIBUTIONS_UNIFORM_H

#include <algorithm>
#include <random>

namespace K {

	class UniformDistribution {

	private:

		std::random_device rd;
		std::mt19937 gen;
		std::uniform_real_distribution<double> dist;

	public:

		/** ctor */
		UniformDistribution(const double min, const double max) : gen(rd()), dist(min, max) {
			;
		}

		/** get a normally distributed random number */
		double draw() {
			return dist(gen);
		}

		/** set the seed to use */
		void setSeed(const long seed) {
			gen.seed(seed);
		}

	};

}

#endif // K_MATH_DISTRIBUTIONS_UNIFORM_H
