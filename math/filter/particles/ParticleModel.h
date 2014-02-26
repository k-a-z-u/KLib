/*
 * ParticleModel.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLEMODEL_H_
#define PARTICLEMODEL_H_

/**
 * interface for all (modification) models, adjusting
 * the particles over time (e.g. movement)
 */
template <typename State> class ParticleModel {

public:

	/** dtor */
	virtual ~ParticleModel() {;}

	/**
	 * the actor modifies the given state (e.g. by moving the robot forward)
	 * @param s the state to modify
	 */
	virtual void modify(State& s) const = 0;

	/**
	 * initialize the given state (e.g. using a normal distribution, even-distribution, ...)
	 * @param s the state to initialize
	 */
	virtual void init(State& s) const = 0;

};


#endif /* PARTICLEMODEL_H_ */
