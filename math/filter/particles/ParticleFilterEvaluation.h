#ifndef K_MATH_FILTERS_PARTICLE_PARTICLEFILTEREVALUATION_H
#define K_MATH_FILTERS_PARTICLE_PARTICLEFILTEREVALUATION_H

#include <vector>

#include "Particle.h"

namespace K {

	/**
	 * interface for the user-defined particle-evaluation.
	 * the evaluation weighs the particle by comparing its state with the observation p(o_t | q_t)
	 */
	template <typename State, typename Observation>
	class ParticleFilterEvaluation {

	public:

		/** evaluate all particles depending on their state and the current observation */
		virtual void evaluation(std::vector<Particle<State>>& particles, const Observation& observation) = 0;

	};

}

#endif // K_MATH_FILTERS_PARTICLE_PARTICLEFILTEREVALUATION_H
