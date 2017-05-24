#ifndef PARTICLEFILTERRESAMPLINGNEFF_H
#define PARTICLEFILTERRESAMPLINGNEFF_H



#include <algorithm>
#include <functional>

#include "ParticleFilterResampling.h"
#include "../ParticleAssertions.h"

namespace K {

	/**
	 * TODO
	 */
	template <typename State> class ParticleFilterResamplingNEff : public ParticleFilterResampling<State> {

	public:

		using DrawCallback = std::function<void(Particle<State>& p)>;

	private:

		/** random number generator */
		std::minstd_rand gen;

		const float desiredNEff;

		const float maxAdapt = 0.05;

		DrawCallback drawCallback;

	public:

		/** ctor */
		ParticleFilterResamplingNEff(const float desiredNEff = 0.75, const float maxAdapt = 0.05) : desiredNEff(desiredNEff), maxAdapt(maxAdapt) {
			gen.seed(1234);
		}

		/** callback to inform about redrawn particles */
		void setDrawCallback(const DrawCallback& callback) {this->drawCallback = callback;}

		void resample(std::vector<Particle<State>>& particles) override {

			// comparator (highest first)
			static auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) {
				return p1.weight > p2.weight;
			};

			// ensure all particle-weights sum up to 1.0
			normalize(particles);

			// calculate current N_eff
			const double curNEff = getNEff(particles);

			// cur N_eff is > desired N_eff -> nothing to do
			if (curNEff > desiredNEff) {return;}

			const size_t cnt = particles.size();

			// how many particles to discard and resample based on N_eff
			// example: N_eff = 75% -> 25% to discard/resample
			// cur N_eff < desired N_eff -> calculate adaption
			float adapt = desiredNEff - curNEff;
			if (adapt > maxAdapt) {adapt = maxAdapt;}
			const size_t toResample = cnt * adapt;



			// sort orig particles by weight (highest first)
			std::sort(particles.begin(), particles.end(), comp);

			// to-be-removed region [at the end of the vector]
			const size_t start = particles.size() - toResample;

			// zero the weight of the to-be-removed region
			for (size_t i = start; i < cnt; ++i) {
				particles[i].weight = 0;
			}

			// renormalize the remaining set
			normalize(particles);

			// replace the to-be-removed region [start:cnt]
			for (size_t i = start; i < cnt; ++i) {

				// track cumulative weight for each particle-index
				std::vector<float> cumWeights;

				// calculate cumulative weight for the copy	[0:start] = the to-be-kept particles
				double cumWeight = 0;
				for (size_t i = 0; i < start; ++i) {
					cumWeight += particles[i].weight;
					cumWeights.push_back(cumWeight);
				}

				// get a random particle between [0:start]
				const size_t idx = draw(cumWeights);

				// split
				particles[i] = particles[idx];
				particles[i].weight /= 2;
				particles[idx].weight /= 2;

				// callback?
				if (drawCallback) {drawCallback(particles[i]);}

			}

			// sanity check. sum of weights must now still be 1.0!
			double weightSum = 0;
			for (const auto& p : particles) {weightSum += p.weight;}

			if (std::abs(weightSum - 1.0) > 0.01) {
				throw Exception("particle weight does not sum up to 1.0");
			}

		}

	private:

		void normalize(std::vector<Particle<State>>& particles) const  {
			double sum = 0.0;
			for (const auto& p : particles) {sum += p.weight;}	// calculate sum
			for (auto& p : particles) {p.weight /= sum;}		// normalize
		}

		double getNEff(const std::vector<Particle<State>>& particles) const {
			double sum = 0.0;
			for (auto& p : particles) {
				sum += (p.weight * p.weight);
			}
			return 1.0 / sum / particles.size();
		}

		size_t draw(const std::vector<float>& cumWeights) {

			// get the cumulative weight [= last entry]
			const double cumWeight = cumWeights.back();

			// generate random values between [0:cumWeight]
			std::uniform_real_distribution<float> dist(0, cumWeight);

			// draw a random value between [0:cumWeight]
			const float rand = dist(gen);

			// search comparator (cumWeight is ordered -> use binary search)
			auto it = std::lower_bound(cumWeights.begin(), cumWeights.end(), rand);

			// convert iterator to index
			const int idx = it - cumWeights.begin();

			return idx;

		}

	};

}


#endif // PARTICLEFILTERRESAMPLINGNEFF_H
