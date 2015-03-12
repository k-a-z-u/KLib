#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONREGIONALWEIGHTEDAVERAGE_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONREGIONALWEIGHTEDAVERAGE_H

#include "ParticleFilterEstimation.h"
#include "../Particle.h"
#include "../ParticleAssertions.h"

#include <algorithm>

namespace K {

	/**
	 * this implementation estimates the current state
	 * by using the most probable particle and some near particles
	 * combining them by their weight (weighted average)
	 *
	 * the checker, whether a particle is near or not, uses a special,
	 * user-defined metric "belongsToRegion()". This user-method must
	 * return a boolean, whether to include the provided particle
	 * within the region around the maximum particle, or not.
	 */
	template <typename State>
	class ParticleFilterEstimationRegionalWeightedAverage : public ParticleFilterEstimation<State> {

	public:

		State estimate(std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			static_assert( HasOperatorPlusEq<State>::value, "your state needs a += operator!" );
			static_assert( HasOperatorDivEq<State>::value, "your state needs a /= operator!" );
			static_assert( HasOperatorMul<State>::value, "your state needs a * operator!" );

			//1) get the most probable particle
			const auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {return p1.weight < p2.weight;};
			const Particle<State>& max = *std::max_element(particles.begin(), particles.end(), comp);

			//2) calculate the regional weighted average
			double cumWeight = 0;
			State res;
			for (const Particle<State>& p : particles) {
				if (!p.state.belongsToRegion(max.state)) {continue;}
				cumWeight += p.weight;
				res += p.state * p.weight;
			}

			// 3) normalize
			res /= cumWeight;

			// done
			return res;

		}


	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONREGIONALWEIGHTEDAVERAGE_H
