/*
 * ParticleResampling.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLING_H_
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLING_H_

#include "../Particle.h"
#include <vector>

/**
 * interface for all available resampling methods
 * within the particle filter
 * @param State the (user-defined) state
 */

namespace K {

	template <typename State> class ParticleFilterResampling {

	public:

		/**
		 * perform resampling on the given particle-vector
		 * @param particles the vector of all particles to resample
		 */
		virtual void resample(std::vector<K::Particle<State>>& particles) = 0;

	};

}

#endif /* K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLING_H_ */
