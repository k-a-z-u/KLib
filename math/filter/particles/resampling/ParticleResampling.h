/*
 * ParticleResampling.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLERESAMPLING_H_
#define PARTICLERESAMPLING_H_

#include "../Particle.h"
#include <vector>

/**
 * interface for particle resamplers
 */
template <typename State> class ParticleResampling {

public:

	/** dtor */
	virtual ~ParticleResampling() {;}

	/**
	 * perform resampling on the given particle-vector
	 * @param particles the vector of all particles to resample
	 */
	virtual void resample(std::vector<Particle<State>>& particles) const = 0;

};

#endif /* PARTICLERESAMPLING_H_ */
