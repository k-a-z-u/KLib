/*
 * ParticleFilter.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <vector>
#include <memory>

#include "Particle.h"

#include "resampling/ParticleFilterResampling.h"
#include "estimation/ParticleFilterEstimation.h"
#include "ParticleFilterTransition.h"
#include "ParticleFilterEvaluation.h"
#include "ParticleFilterInitializer.h"

namespace K {

	/**
	 * the main-class for the particle filter
	 * @param State the (user-defined) state for each particle
	 * @param Observation the observation (sensor) data
	 */
	template <typename State, typename Observation>

	class ParticleFilter {

	private:

		/** all used particles */
		std::vector<Particle<State>> particles;

		/** the resampler to use */
		std::unique_ptr<ParticleFilterResampling<State>> resampler;

		/** the estimation function to use */
		std::unique_ptr<ParticleFilterEstimation<State>> estimation;

		/** the transition function to use */
		std::unique_ptr<ParticleFilterTransition<State>> transition;

		/** the evaluation function to use */
		std::unique_ptr<ParticleFilterEvaluation<State, Observation>> evaluation;

		/** the initialization function to use */
		std::unique_ptr<ParticleFilterInitializer<State>> initializer;



	public:

		/** ctor */
		ParticleFilter(const uint32_t numParticles, std::unique_ptr<ParticleFilterInitializer<State>> initializer) {
			particles.resize(numParticles);
			setInitializier(std::move(initializer));
			init();
		}

		/** dtor */
		~ParticleFilter() {
			;
		}

		/** access to all particles */
		const std::vector<Particle<State>>& getParticles() {
			return particles;
		}

		/** initialize/re-start the particle filter */
		void init() {
			initializer->initialize(particles);
		}


		/** set the resampling method to use */
		void setResampling(std::unique_ptr<ParticleFilterResampling<State>> resampling) {
			this->resampler = std::move(resampling);
		}

		/** set the estimation method to use */
		void setEstimation(std::unique_ptr<ParticleFilterEstimation<State>> estimation) {
			this->estimation = std::move(estimation);
		}

		/** set the transition method to use */
		void setTransition(std::unique_ptr<ParticleFilterTransition<State>> transition) {
			this->transition = std::move(transition);
		}

		/** set the evaluation method to use */
		void setEvaluation(std::unique_ptr<ParticleFilterEvaluation<State, Observation>> evaluation) {
			this->evaluation = std::move(evaluation);
		}

		/** set the initialization method to use */
		void setInitializier(std::unique_ptr<ParticleFilterInitializer<State>> initializer) {
			this->initializer = std::move(initializer);
		}

		/** perform resampling -> transition -> evaluation -> estimation */
		State update(const Observation& observation) {
			resampler->resample(particles);
			transition->transition(particles);
			evaluation->evaluation(particles, observation);
			return estimation->estimate(particles);
		}

	};

}

#endif /* PARTICLEFILTER_H_ */
