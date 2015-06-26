#ifndef PARTICLEFILTERESTIMATIONORDEREDWEIGHTEDAVERAGE_H
#define PARTICLEFILTERESTIMATIONORDEREDWEIGHTEDAVERAGE_H


#include <vector>
#include "../Particle.h"
#include "../ParticleAssertions.h"
#include "ParticleFilterEstimation.h"

#include "../../../../Assertions.h"

namespace K {

	/**
	 * calculate the (weighted) average state using
	 * the X% best weighted particles
	 */
	template <typename State>
	class ParticleFilterEstimationOrderedWeightedAverage : public ParticleFilterEstimation<State> {

	private:

		const float percent;

	public:

		/** ctor */
		ParticleFilterEstimationOrderedWeightedAverage(const float percent) : percent(percent) {;}

		State estimate(std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			static_assert( HasOperatorPlusEq<State>::value, "your state needs a += operator!" );
			static_assert( HasOperatorDivEq<State>::value, "your state needs a /= operator!" );
			static_assert( HasOperatorMul<State>::value, "your state needs a * operator!" );

			// comparator (highest first)
			auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {
				return p1.weight > p2.weight;
			};

			// sort
			std::sort (particles.begin(), particles.end(), comp);

			State tmp;

			// calculate weighted average
			const int numBest = particles.size() * percent;
			double weightSum = 0;
			for (int i = 0; i < numBest; ++i) {
				const Particle<State>& p = particles[i];
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

#endif // PARTICLEFILTERESTIMATIONORDEREDWEIGHTEDAVERAGE_H
