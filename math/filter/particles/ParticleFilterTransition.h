#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H

#include <vector>
#include "Particle.h"

namespace K {

	/**
	 * interface for the user-defined particle transition.
	 * the transition describes the particles change during the transition phase p(q_t | q_t-1)
	 * depending on the control data (if any)
	 */
	template <typename State, typename Control>
	class ParticleFilterTransition {

	public:

		/** perform the transition p(q_t | q_t-1) for all particles based on the given control data */
		virtual void transition(std::vector<Particle<State>>& particles, const Control* control) = 0;

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERTRANSITION_H
