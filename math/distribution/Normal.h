#ifndef K_MATH_DISTRIBUTION_NORMAL_H
#define K_MATH_DISTRIBUTION_NORMAL_H

#include <algorithm>
#include <random>
#include "../../Assertions.h"

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
		NormalDistribution(const double mu, const double sigma) :
			mu(mu), sigma(sigma), _a(1.0 / (sigma * std::sqrt(2.0 * M_PI))), gen(rd()), dist(mu,sigma) {

			_assertNotNAN(mu, "mu is NaN");
			_assertNotNAN(sigma, "sigma is NaN");

		}

		/** copy ctor */
		NormalDistribution(const NormalDistribution& nd) : mu(nd.mu), sigma(nd.sigma), _a(nd._a) {

			_assertNotNAN(mu, "mu is NaN");
			_assertNotNAN(sigma, "sigma is NaN");

		}

		/** get probability for the given value */
		double getProbability(const double val) const {
			const double b = -0.5 * ((val-mu)/sigma) * ((val-mu)/sigma);
			return _a * std::exp(b);
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
		static double getProbability(const double mu, const double sigma, const double val) {
			const double a = 1.0 / (sigma * std::sqrt(2.0 * M_PI));
			const double b = -0.5 * ((val-mu)/sigma) * ((val-mu)/sigma);
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

	/** cumulative density version of the normal distribution */
	class NormalDistributionCDF {

	private:

		const double mu;
		const double sigma;

	public:

		/** create a new normally distributed CDF */
		NormalDistributionCDF(const double mu, const double sigma) : mu(mu), sigma(sigma) {
			;
		}

		/** get the probability for val within the underlying CDF */
		double getProbability(const double val) const {
			return getProbability(mu, sigma, val);
		}

		/** calculate the probability within the underlying CDF */
		static double getProbability(const double mu, const double sigma, const double val) {
			return (1.0 + std::erf( (val - mu) / (sigma * std::sqrt(2)) ) ) / 2.0;
		}

	};

}

#endif // K_MATH_DISTRIBUTION_NORMAL_H
