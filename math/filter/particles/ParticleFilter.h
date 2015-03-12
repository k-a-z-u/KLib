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

#include "../../../Assertions.h"

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

		/** the percentage-of-efficient-particles-threshold for resampling */
		double nEffThresholdPercent = 0.25;



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
			_assertNotNull(initializer, "initializer MUST not be null! call setInitializer() first!");
			initializer->initialize(particles);
		}


		/** set the resampling method to use */
		void setResampling(std::unique_ptr<ParticleFilterResampling<State>> resampler) {
			_assertNotNull(resampler, "setResampling() MUST not be called with a nullptr!");
			this->resampler = std::move(resampler);
		}

		/** set the estimation method to use */
		void setEstimation(std::unique_ptr<ParticleFilterEstimation<State>> estimation) {
			_assertNotNull(estimation, "setEstimation() MUST not be called with a nullptr!");
			this->estimation = std::move(estimation);
		}

		/** set the transition method to use */
		void setTransition(std::unique_ptr<ParticleFilterTransition<State>> transition) {
			_assertNotNull(transition, "setTransition() MUST not be called with a nullptr!");
			this->transition = std::move(transition);
		}

		/** get the used transition method */
		ParticleFilterTransition<State>* getTransition() {
			return this->transition.get();
		}

		/** set the evaluation method to use */
		void setEvaluation(std::unique_ptr<ParticleFilterEvaluation<State, Observation>> evaluation) {
			_assertNotNull(evaluation, "setEvaluation() MUST not be called with a nullptr!");
			this->evaluation = std::move(evaluation);
		}

		/** set the initialization method to use */
		void setInitializier(std::unique_ptr<ParticleFilterInitializer<State>> initializer) {
			_assertNotNull(initializer, "setInitializer() MUST not be called with a nullptr!");
			this->initializer = std::move(initializer);
		}

		/** set the resampling threshold as the percentage of efficient particles */
		void setNEffThreshold(const double thresholdPercent) {
			this->nEffThresholdPercent = thresholdPercent;
		}

		/** perform resampling -> transition -> evaluation -> estimation */
		State update(const Observation& observation) {

			// sanity checks (if enabled)
			_assertNotNull(resampler, "resampler MUST not be null! call setResampler() first!");
			_assertNotNull(transition, "transition MUST not be null! call setTransition() first!");
			_assertNotNull(evaluation, "evaluation MUST not be null! call setEvaluation() first!");
			_assertNotNull(estimation, "estimation MUST not be null! call setEstimation() first!");

			// perform the transition step
			transition->transition(particles);

			// perform the evaluation step and calculate the sum of all particle weights
			const double weightSum = evaluation->evaluation(particles, observation);

			// normalize the particle weights and thereby calculate N_eff
			const double neff = normalize(weightSum);

			// estimate the current state
			const State est = estimation->estimate(particles);

			// if the number of efficient particles is too low, perform resampling
			if (neff < particles.size() * nEffThresholdPercent) { resampler->resample(particles); }

			// done
			return est;

		}

		/** perform only the transition step */
		void updateTransitionOnly() {

			// sanity checks (if enabled)
			_assertNotNull(transition, "transition MUST not be null! call setTransition() first!");

			// perform the transition step
			transition->transition(particles);

		}

		/** perform only the evaluation step */
		State updateEvaluationOnly(const Observation& observation) {

			// sanity checks (if enabled)
			_assertNotNull(resampler, "resampler MUST not be null! call setResampler() first!");
			_assertNotNull(evaluation, "evaluation MUST not be null! call setEvaluation() first!");
			_assertNotNull(estimation, "estimation MUST not be null! call setEstimation() first!");

			// perform the evaluation step and calculate the sum of all particle weights
			const double weightSum = evaluation->evaluation(particles, observation);

			// normalize the particle weights and thereby calculate N_eff
			const double neff = normalize(weightSum);

			// estimate the current state
			const State est = estimation->estimate(particles);

			// if the number of efficient particles is too low, perform resampling
			if (neff < particles.size() * nEffThresholdPercent) { resampler->resample(particles); }

			// done
			return est;

		}

	private:

		/** normalize the weight of all particles to one */
		double normalize(const double weightSum) {
			double sum = 0.0;
			for (auto& p : particles) {
				p.weight /= weightSum;
				sum += (p.weight * p.weight);
			}
			return 1.0 / sum;
		}

		/** calculate the number of efficient particles (N_eff) */
		double getNeff() const {
			double sum = 0.0;
			for (auto& p : particles) {sum += (p.weight * p.weight);}
			return 1.0 / sum;
		}
	};

}

#endif /* PARTICLEFILTER_H_ */
