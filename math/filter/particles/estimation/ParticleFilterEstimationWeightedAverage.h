#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H

#include <vector>
#include "../Particle.h"
#include "../ParticleAssertions.h"
#include "ParticleFilterEstimation.h"

#include "../../../../Assertions.h"

namespace K {

	/**
	 * calculate the (weighted) average state using
	 * all particles and their weight
	 */
	template <typename State>
	class ParticleFilterEstimationWeightedAverage : public ParticleFilterEstimation<State> {

	public:

		State estimate(const std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			static_assert( HasOperatorPlusEq<State>::value, "your state needs a += operator!" );
			static_assert( HasOperatorDivEq<State>::value, "your state needs a /= operator!" );
			static_assert( HasOperatorMul<State>::value, "your state needs a * operator!" );

			State tmp;

			// calculate weighted average
			double weightSum = 0;
			for (const Particle<State>& p : particles) {
				tmp += p.state * p.weight;
				weightSum += p.weight;
			}

			_assertTrue( (weightSum == weightSum), "the sum of particle weights is NaN!");
			_assertTrue( (weightSum != 0), "the sum of particle weights is null!");

			// normalize
			tmp /= weightSum;

			return tmp;

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H
