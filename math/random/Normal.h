#ifndef K_MATH_RND_NORMAL_H
#define K_MATH_RND_NORMAL_H

namespace K {


	/**
	 * provides some common functions
	 * for handling normal-distributed random numbers
	 */
	class Normal {

	public:

		/** get normal-distributed random number for given mu/sigma */
		static double get(double mu, double sigma) {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::normal_distribution<> d(mu, sigma);
			return d(gen);
		}

	};

}

#endif // K_MATH_RND_NORMAL_H
