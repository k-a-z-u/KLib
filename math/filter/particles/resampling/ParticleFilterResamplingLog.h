#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGLOG_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGLOG_H

#include "ParticleFilterResamplingSimple.h"

namespace K {

	/**
	 * draw particles using their log-weight
	 */
	template <typename State>
	class ParticleFilterResamplingLog : public ParticleFilterResamplingSimple<State> {

	public:

		void resample(std::vector<Particle<State>>& particles) override {

			for (Particle<State>& p : particles) {
				//p.weight = - 1.0 / std::log(p.weight);
				p.weight = std::pow(p.weight, 0.5);
			}

			ParticleFilterResamplingSimple<State>::resample(particles);

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGLOG_H
