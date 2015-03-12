/*
 * ParticleResamplingSimple.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGSIMPLE_H_
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGSIMPLE_H_

#include <algorithm>

#include "ParticleFilterResampling.h"
#include "../ParticleAssertions.h"

namespace K {

	/**
	 * uses simple probability resampling by drawing particles according
	 * to their current weight.
	 * O(log(n)) per particle
	 */
	template <typename State>
	class ParticleFilterResamplingSimple : public ParticleFilterResampling<State> {

	private:

		/** this is a copy of the particle-set to draw from it */
		std::vector<Particle<State>> particlesCopy;

	public:

		void resample(std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			static_assert( HasOperatorAssign<State>::value, "your state needs an assignment operator!" );

			const uint32_t cnt = (uint32_t) particles.size();

			// egal weight for all particles. sums up to 1.0
			const double equalWeight = 1.0 / (double) cnt;

			// ensure the copy vector has the same size as the real particle vector
			particlesCopy.resize(cnt);

			// swap both vectors
			particlesCopy.swap(particles);

			// calculate cumulative weight
			double cumWeight = 0;
			for (uint32_t i = 0; i < cnt; ++i) {
				cumWeight += particlesCopy[i].weight;
				particlesCopy[i].weight = cumWeight;
			}

			// now draw from the copy vector and fill the original one
			// with the resampled particle-set
			for (uint32_t i = 0; i < cnt; ++i) {
				particles[i] = draw(cumWeight);
				particles[i].weight = equalWeight;
			}

		}

	private:

		/** draw one particle according to its weight from the copy vector */
		const Particle<State>& draw(const double cumWeight) const {

			// random value between [0;1]
			const double rand01 = double(rand()) / double(RAND_MAX);

			// random value between [0; cumulativeWeight]
			const double rand = rand01 * cumWeight;

			// search comparator
			auto comp = [] (const Particle<State>& s, const double d) {return s.weight < d;};
			auto it = std::lower_bound(particlesCopy.begin(), particlesCopy.end(), rand, comp);
			return *it;

		}



	};


}



#endif /* K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGSIMPLE_H_ */
