/*
 * ParticleSensor.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLESENSOR_H_
#define PARTICLESENSOR_H_


/**
 * interface for all particle-filter sensors
 */
template <typename State> class ParticleSensor {

public:

	/** dtor */
	virtual ~ParticleSensor() {;}

	/** get the probability for the sensor measuring the given state */
	virtual double getProbability(const State& s) const = 0;

};

#endif /* PARTICLESENSOR_H_ */
