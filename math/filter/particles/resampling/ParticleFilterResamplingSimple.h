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
#include "../../../../os/Time.h"

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

		/** random number generator */
		std::minstd_rand gen;

	public:

		/** ctor */
		ParticleFilterResamplingSimple() {
			gen.seed(1234);
		}

		void resample(std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			// TODO: this solution requires EXPLICIT overloading which is bad...
			//static_assert( HasOperatorAssign<State>::value, "your state needs an assignment operator!" );

			const uint32_t cnt = (uint32_t) particles.size();

			// equal weight for all particles. sums up to 1.0
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
		const Particle<State>& draw(const double cumWeight) {

			// generate random values between [0:cumWeight]
			std::uniform_real_distribution<float> dist(0, cumWeight);

			// draw a random value between [0:cumWeight]
			const float rand = dist(gen);

			// search comparator (cumWeight is ordered -> use binary search)
			auto comp = [] (const Particle<State>& s, const float d) {return s.weight < d;};
			auto it = std::lower_bound(particlesCopy.begin(), particlesCopy.end(), rand, comp);
			return *it;

		}



	};


}



#endif /* K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGSIMPLE_H_ */
