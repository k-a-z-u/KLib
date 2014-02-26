/*
 * Particle.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

/**
 * extends a particle's state (some parameters, e.g. (x,y) position)
 * by a probability
 */
template <typename State> struct Particle {

	/** ctor */
	Particle() : probability(0) {;}

	/** the (current) probability for this state */
	double probability;

	/** the particles state */
	State state;

};

#endif /* PARTICLE_H_ */
