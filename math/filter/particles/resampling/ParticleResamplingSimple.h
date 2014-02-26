/*
 * ParticleResamplingSimple.h
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLERESAMPLINGSIMPLE_H_
#define PARTICLERESAMPLINGSIMPLE_H_

#include "ParticleResampling.h"
#include "../ParticleMath.h"
#include <algorithm>
/**
 * uses simple probability resampling
 *
 * TODO optimize algorithm.. currently O(n²)
 */
template <typename State> class ParticleResamplingSimple : public ParticleResampling<State> {

public:

	/** dtor */
	~ParticleResamplingSimple() {;}

	void resample(std::vector<Particle<State>>& particles) const override {

		const size_t numParticles = particles.size();
		//const float newProbability = 1.0 / numParticles;

		// create a copy of the current particle-set
		std::vector<Particle<State>> copy(particles);

		// sort copy by probability
		//sortByProbabilityDESC(copy);

		// now, create the new set of particles
		// by randomly drawing particles from the current set
		for (size_t i = 0; i < numParticles; ++i) {

			// pick a random probability (between [0;1])
			double rnd = ParticleMath::getRandom(0,1);

			// cumulative probability
			// sum-up until 'rnd' is reached -> use current particle
			double cumProbability = 0;

			// now sum up weights of particles until the random number is lower
			for (unsigned int j = 0; j < numParticles; ++j) {

				Particle<State>& p = copy[j];
				cumProbability += p.probability;

				// random-probability reached?
				if (cumProbability >= rnd) {
					particles[i] = p;
					break;
				}

			}

		}

	}

private:

//	/** sort all particles by their current probability (descending) */
//	void sortByProbabilityDESC(std::vector<Particle<State>>& particles) const {
//
//		// comparator
//		const auto& lambda = [] (const Particle<State>& a, const Particle<State>& b) {
//			return a.probability > b.probability;
//		};
//
//		// sort
//		std::sort(particles.begin(), particles.end(), lambda);
//
//	}

};





#endif /* PARTICLERESAMPLINGSIMPLE_H_ */
