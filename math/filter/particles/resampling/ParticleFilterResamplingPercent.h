#ifndef PARTICLEFILTERRESAMPLINGPERCENT_H
#define PARTICLEFILTERRESAMPLINGPERCENT_H


#include <algorithm>

#include "ParticleFilterResampling.h"
#include "../ParticleAssertions.h"

namespace K {

	/**
	 * TODO
	 */
	template <typename State> class ParticleFilterResamplingPercent : public ParticleFilterResampling<State> {

	private:

		/** random number generator */
		std::minstd_rand gen;

		float percent;

	public:

		/** ctor */
		ParticleFilterResamplingPercent(const float percent) : percent(percent) {
			gen.seed(1234);
		}

		void resample(std::vector<Particle<State>>& particles) override {

			// comparator (highest first)
			static auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {
				return p1.weight > p2.weight;
			};

			const uint32_t cnt = (uint32_t) particles.size();

			// sort particles by weight (highest first)
			std::sort(particles.begin(), particles.end(), comp);


			// to-be-removed region
			const int start = particles.size() * (1-percent);
			const int end = particles.size();
			std::uniform_int_distribution<int> dist(0, start-1);

			// remove by re-drawing
			for (uint32_t i = start; i < end; ++i) {
				const int rnd = dist(gen);
				particles[i] = particles[rnd];
				particles[i].weight /= 2;
				particles[rnd].weight /= 2;
			}

			// calculate weight-sum
			double weightSum = 0;
			for (const auto& p : particles) {
				weightSum += p.weight;
			}


			int i = 0;



		}

	private:

		/** draw one particle according to its weight from the copy vector */
		const Particle<State>& draw(std::vector<Particle<State>>& copy, const double cumWeight) {

			// generate random values between [0:cumWeight]
			std::uniform_real_distribution<float> dist(0, cumWeight);

			// draw a random value between [0:cumWeight]
			const float rand = dist(gen);

			// search comparator (cumWeight is ordered -> use binary search)
			auto comp = [] (const Particle<State>& s, const float d) {return s.weight < d;};
			auto it = std::lower_bound(copy.begin(), copy.end(), rand, comp);
			return *it;

		}



	};

}

#endif // PARTICLEFILTERRESAMPLINGPERCENT_H
