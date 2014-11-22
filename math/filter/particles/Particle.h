/*
 * Particle.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

/**
 * a particle consists of a (user-defined) state
 * assigned with a weight (importance).
 *
 * depending on the particle filter's configuration,
 * the (user-defined) state must provide several methods
 * like:
 *	assigning values from another state
 *	multiplication
 *	etc..
 *
 */
template <typename State> struct Particle {

	/** the particles state */
	State state;

	/** the (current) probability for this state */
	double weight;


	/** empty ctor */
	Particle() : state(), weight(0) {;}

	/** ctor */
	Particle(const State& state, double weight) : state(state), weight(weight) {;}


	/** assignment operator */
	Particle& operator = (const Particle& other) {
		this->state = other.state;
		this->weight = other.weight;
		return *this;
	}


};

#endif /* PARTICLE_H_ */
