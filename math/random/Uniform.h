#ifndef K_MATH_RANDOM_UNIFORM_H
#define K_MATH_RANDOM_UNIFORM_H

#include "RandomGenerator.h"

namespace K {

	/**
	 * provides some common functions
	 * for handling normal-distributed random numbers
	 */
	template <typename T> class Uniform {

	private:

		RandomGenerator gen;

		/** depending on T, Dist is either a uniform_real or uniform_int distribution */
		typedef typename std::conditional< std::is_floating_point<T>::value, std::uniform_real_distribution<T>, std::uniform_int_distribution<T> >::type Dist;
		Dist dist;

	public:

		/** ctor */
		Uniform(const T min, const T max) : gen(RANDOM_SEED), dist(min, max) {

		}

		/** get a uniformaly distributed random number */
		T draw() {
			return dist(gen);
		}

		/** set the seed to use */
		void setSeed(const long seed) {
			gen.seed(seed);
		}

	};

}

#endif // K_MATH_RANDOM_UNIFORM_H
