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

		/** empty ctor: (0,1) */
		UniformDistribution() : gen(rd()), dist(0, 1) {
			;
		}

		/** ctor */
		UniformDistribution(const double min, const double max) : gen(rd()), dist(min, max) {
			;
		}

		/** reconfigure min/max */
		void reset(const double min, const double max) {
			dist = std::uniform_real_distribution<double>(min, max);
		}

		/** get a normally distributed random number */
		double draw() {
			return dist(gen);
		}

		/** set the seed to use */
		void setSeed(const long seed) {
			gen.seed(seed);
		}

	public:

		/** convenience method to just draw one random number between min and max */
		template <typename T> static T draw(const T min, const T max) {
			return min + (rand() * (max-min) / RAND_MAX);
		}

	};

}

#endif // K_MATH_DISTRIBUTIONS_UNIFORM_H
