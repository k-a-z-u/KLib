#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H

#include <vector>
#include "../Particle.h"
#include "../ParticleAssertions.h"
#include "ParticleFilterEstimation.h"

namespace K {

	template <typename State>
	class ParticleFilterEstimationWeightedAverage : public ParticleFilterEstimation<State> {

	public:

		State estimate(std::vector<Particle<State>>& particles) override {

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

			// normalize
			tmp /= weightSum;

			return tmp;

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONWEIGHTEDAVERAGE_H
