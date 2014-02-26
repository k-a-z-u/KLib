/*
 * ParticleResamplingNone.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLERESAMPLINGNONE_H_
#define PARTICLERESAMPLINGNONE_H_

#include "ParticleResampling.h"

/**
 * dummy resampler
 */
template <typename State> class ParticleResamplingNone : public ParticleResampling<State> {

public:

	/** dtor */
	~ParticleResamplingNone() {;}

	void resample(std::vector<Particle<State>>& particles) const override {
		(void) particles;
	}

};


#endif /* PARTICLERESAMPLINGNONE_H_ */
