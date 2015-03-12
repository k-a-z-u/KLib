#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERINITIALIZER_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERINITIALIZER_H

#include <vector>
#include "Particle.h"

namespace K {

	/**
	 * interface for particle filter initializers.
	 * an initializer "configures" all particles before the filter starts: p(q_0)
	 */
	template <typename State>
	class ParticleFilterInitializer {

	public:

		/** the initializer must setup each particle within the given vector */
		virtual void initialize(std::vector<Particle<State>>& particles) = 0;

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERINITIALIZER_H
