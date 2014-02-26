/*
 * ParticleMath.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLEMATH_H_
#define PARTICLEMATH_H_

#include <cmath>
#include <random>

/**
 * some often-needed helper-functions for the
 * particle-filter backend
 */
class ParticleMath {

public:

	/**
	 * draw a random value using a given normal-distribution
	 * @param mu the average of the normal-distribution
	 * @param sigma the std-deviation of the normal-distribution
	 */
	static double getNormalDistributionRandom(double mu, double sigma) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::normal_distribution<> d(mu, sigma);
		return d(gen);
	}

	/**
	 * get a random probability between min and max
	 * @param min the minimum value to return (including)
	 * @param max the maximum value to return (including)
	 */
	static double getRandom(double min, double max) {
		return (min + rand() / (double) RAND_MAX * (max-min));
	}

};


#endif /* PARTICLEMATH_H_ */
