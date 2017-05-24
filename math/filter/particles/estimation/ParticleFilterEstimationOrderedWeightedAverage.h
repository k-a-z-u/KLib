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

		State estimate(const std::vector<Particle<State>>& particles) override {

			// compile-time sanity checks
			static_assert( HasOperatorPlusEq<State>::value, "your state needs a += operator!" );
			static_assert( HasOperatorDivEq<State>::value, "your state needs a /= operator!" );
			static_assert( HasOperatorMul<State>::value, "your state needs a * operator!" );

			// comparator (highest first)
			auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {
				return p1.weight > p2.weight;
			};

			// create a copy
			std::vector<Particle<State>> copy;
			copy.insert(copy.begin(), particles.begin(), particles.end());

			// sort the copy (highest weight first)
			std::sort (copy.begin(), copy.end(), comp);

			State tmp;

			// calculate weighted average
			const int numBest = copy.size() * percent;
			double weightSum = 0;
			for (int i = 0; i < numBest; ++i) {
				const Particle<State>& p = copy[i];
				tmp += p.state * p.weight;
				weightSum += p.weight;
			}

			if (weightSum != weightSum) {
				int i = 0; (void) i;
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
