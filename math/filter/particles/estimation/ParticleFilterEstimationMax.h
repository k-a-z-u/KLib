#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONMAX_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONMAX_H

#include "ParticleFilterEstimation.h"
#include <algorithm>
#include <vector>
#include "../Particle.h"

namespace K {

	template <typename State>
	class ParticleFilterEstimationMax : public ParticleFilterEstimation<State> {

	public:

		State estimate(std::vector<Particle<State>>& particles) override {

			// comparator
			auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {
				return p1.weight < p2.weight;
			};

			// find max element
			auto el = std::max_element(particles.begin(), particles.end(), comp);
			return el->state;

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONMAX_H
