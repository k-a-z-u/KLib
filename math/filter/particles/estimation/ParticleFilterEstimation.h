#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATION_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATION_H

#include "../Particle.h"

namespace K {

	template <typename State>
	class ParticleFilterEstimation {

	public:

		// dtor
		//virtual ~ParticleFilterEstimation() {;}

		// get the current state estimation for the given particle set
		virtual State estimate(std::vector<Particle<State>>& particles) = 0;

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATION_H
