/*
 * ParticleResamplingNone.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGNONE_H_
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGNONE_H_

#include "ParticleFilterResampling.h"

namespace K {

	/**
	 * dummy resampler simply doing nothing
	 */
	template <typename State>
	class ParticleFilterResamplingNone : public ParticleFilterResampling<State> {

	public:

//		/** dtor */
//		~ParticleFilterResamplingNone() {;}

		void resample(std::vector<Particle<State>>& particles) override {
			(void) particles;
		}

	};

}

#endif /* K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGNONE_H_ */
