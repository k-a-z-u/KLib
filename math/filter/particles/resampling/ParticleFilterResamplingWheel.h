#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGWHEEL_H_
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGWHEEL_H_

#include <algorithm>
#include <vector>

#include "ParticleFilterResampling.h"
#include "../ParticleAssertions.h"
#include "../../../distribution/Uniform.h"
#include "../../../../os/Time.h"

/**
 * sources:
 * https://www.udacity.com/course/viewer#!/c-cs373/l-48704330/e-48748082/m-48740082
 * https://www.youtube.com/watch?list=PLpUPoM7Rgzi_7YWn14Va2FODh7LzADBSm&feature=player_detailpage&v=kZhOJgooMxI#t=567
 */

namespace K {

	/**
	 * uses simple probability resampling by drawing particles according
	 * to their current weight using a "wheel"
	 * -> O(n) for all particles
	 */
	template <typename State>
	class ParticleFilterResamplingWheel: public ParticleFilterResampling<State> {

	private:

		/** this is a copy of the particle-set to draw from it */
		std::vector<Particle<State>> particlesCopy;

	public:

		void resample(std::vector<Particle<State>>& particles) override {

			uint64_t start = K::Time::getTimeMS();

			// compile-time sanity checks
			// TODO: this solution requires EXPLICIT overloading which is bad...
			//static_assert( HasOperatorAssign<State>::value, "your state needs an assignment operator!" );

			const uint32_t cnt = (uint32_t) particles.size();

			// equal weight for all particles. sums up to 1.0
			const double equalWeight = 1.0 / (double) cnt;



			// get the weight of the "heaviest" particle
			const auto lambda = [] (const Particle<State>& p1, const Particle<State>& p2) {return p1.weight < p2.weight;};
			const Particle<State> max = *std::max_element(particles.begin(), particles.end(), lambda);

			// draw form the input particle set by treating it as a wheel
			K::UniformDistribution dist(0, 2 * max.weight);
			int curIdx = K::UniformDistribution::draw( (int)0, (int)particles.size() - 1);
			double curOffset = dist.draw();

			// ensure the copy vector has the same size as the real particle vector
			particlesCopy.resize(cnt);

			// draw the new set of particles
			for (uint32_t i = 0; i < cnt; ) {

				// found a suitable particle, use it and draw the next random number
				if (particles[curIdx].weight >= curOffset) {
					particlesCopy[i] = particles[curIdx];
					particlesCopy[i].weight = equalWeight;
					curOffset += dist.draw();
					++i;

				// weight to small, subtract the particles weight and move on to the next particle
				} else {
					curOffset -= particles[curIdx].weight;
					curIdx = (curIdx + 1) % (particles.size());
				}

			}

			// swap both vectors
			particlesCopy.swap(particles);

			uint64_t end = K::Time::getTimeMS();
			std::cout << (end-start) << std::endl;

		}

	};


}



#endif /* K_MATH_FILTER_PARTICLES_PARTICLEFILTERRESAMPLINGWHEEL_H_ */
