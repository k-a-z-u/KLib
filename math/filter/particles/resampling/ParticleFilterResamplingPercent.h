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

			// copy the particles
			std::vector<Particle<State>> copy = particles;

			// sort them by weight (highest first)
			std::sort(copy.begin(), copy.end(), comp);

			// remove the last X percent
			const int start = copy.size() * (1-percent);
			for (int idx = start; idx < copy.size(); ++idx) {
				copy[idx].weight = 0;
			}

			// cumulate
			double cumWeight = 0;
			for (Particle<State>& p : copy) {
				cumWeight += p.weight;
				p.weight = cumWeight;
			}

			// re-draw the last X percent
			for (int idx = start; idx < (int) copy.size(); ++idx) {
				copy[idx] = draw(copy, cumWeight);
			}

			// swap copy and input
			std::swap(particles, copy);


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
