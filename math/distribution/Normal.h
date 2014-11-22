#ifndef K_MATH_DISTRIBUTION_NORMAL_H
#define K_MATH_DISTRIBUTION_NORMAL_H

#include <algorithm>
#include <random>

namespace K {

	class NormalDistribution {

	private:

		double mu;
		double sigma;
		double _a;

		std::random_device rd;
		std::mt19937 gen;
		std::normal_distribution<> dist;

	public:

		/** ctor */
		NormalDistribution(double mu, double sigma) : mu(mu), sigma(sigma), gen(rd()), dist(mu,sigma) {
			_a = 1.0 / (sigma * std::sqrt(2.0 * 3.14159265359));
		}


		/** get probability for the given value */
		double getProbability(const double val) const {
			double b = -0.5 * ((val-mu)/sigma) * ((val-mu)/sigma);
			return _a * std::exp(b);
		}

		/** get a normally distributed random number */
		double draw() {
			return dist(gen);
		}


		/** get the probability for the given value */
		static double getProbability(const double mu, const double sigma, const double val) {
			double a = 1.0 / (sigma * std::sqrt(2.0 * 3.14159265359));
			double b = -0.5 * ((val-mu)/sigma) * ((val-mu)/sigma);
			return a * std::exp(b);
		}

		/** get normal-distributed random number for given mu/sigma */
		static double draw(double mu, double sigma) {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::normal_distribution<> d(mu, sigma);
			return d(gen);
		}

	};

}

#endif // K_MATH_DISTRIBUTION_NORMAL_H
