#ifndef K_MATH_RND_UNIQUE_H
#define K_MATH_RND_UNIQUE_H

namespace K {

	/**
	 * provides some common functions
	 * for handling uniquely distributed random numbers
	 */
	class Unique {

	public:

		/** get uniquely distributed random number between min and max */
		static double getBetween(double min, double max) {
			double rnd = (double) rand() / (double) RAND_MAX;
			rnd *= (max-min);
			rnd += min;
			return rnd;
		}

	};

}

#endif // K_MATH_RND_UNIQUE_H
