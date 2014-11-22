#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H

#include <vector>
#include "Particle.h"

namespace K {

	/**
	 * interface for the user-defined particle transition.
	 * the transition describes the particles change during the transition phase p(q_t | q_t-1)
	 */
	template <typename State>
	class ParticleFilterTransition {

	public:

		/** perform the transition p(q_t | q_t-1) for all particles */
		virtual void transition(std::vector<Particle<State>>& particles) = 0;

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H
