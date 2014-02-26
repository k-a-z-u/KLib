/*
 * ParticleFilter.h
 *
 *  Created on: Sep 17, 2013
 *      Author: Frank Ebner
 */

#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <vector>
#include <cmath>

#include "Particle.h"
#include "ParticleModel.h"
#include "ParticleSensor.h"
#include "resampling/ParticleResampling.h"




/**
 * the main-class for the particle filter
 * @param ParticleState the class (struct) describing the particles state
 */
template <typename State> class ParticleFilter {

public:

	/**
	 * ctor
	 * @param model the model to use for particle modification
	 */
	ParticleFilter(ParticleModel<State>& model, ParticleResampling<State>& resampler) :
		model(model), resampler(resampler) {

	}

	/** add a new sensor to the particle filter */
	void addSensor(ParticleSensor<State>* sensor) {
		sensors.push_back(sensor);
	}

	/** initialize the filter */
	void init(unsigned int num) {
		particles.resize(num);
		for (unsigned int i = 0; i < num; ++i) {
			model.init(particles[i].state);
			particles[i].probability = 1.0;
		}
	}

	/**
	 * perform one update step:
	 * 	trigger model
	 * 	get probability from sensors
	 * 	resample
	 */
	void update() {
		updateModel();
		importance();			// get particle importance by comparing sensor-data and state
		normalize();			// normalize probability-sum to 1 (needed for resampling to work)
		resample();				// draw from particles by their probability (= remove weak particles)
	}

	/** get vector containing all particles */
	const std::vector<Particle<State>>& getParticles() const {
		return particles;
	}

	/**
	 * get the mean state by suming-up all states,
	 * multiplied by their probability
	 */
	State getMean() const {

		const size_t numParticles = particles.size();

		// we start with the first particle instead of an empty one (which would need less code)
		// to avoid issues due to a missing or faulty empty-ctor,
		// initializing all state-attributes with zero
		State res = particles[0].state;
		for (size_t i = 1; i < numParticles; ++i) {res += particles[i].state;}
		res /= (double) numParticles;
		return res;

	}

	/**
	 * get the mean state by suming-up all states,
	 * multiplied by their probability
	 */
	State getWeightedMean() const {

		const size_t numParticles = particles.size();

		// we start with the first particle instead of an empty one (which would need less code)
		// to avoid issues due to a missing or faulty empty-ctor,
		// initializing all state-attributes with zero
		double sum = particles[0].probability;
		State res = particles[0].state * particles[0].probability;
		for (size_t i = 1; i < numParticles; ++i) {
			res += particles[i].state * particles[i].probability;
			sum += particles[i].probability;
		}

		// normalize
		res /= sum;
		return res;

	}

private:

	/** update the particles using the underlying model */
	void updateModel() {
		for (Particle<State>& particle : particles) {
			model.modify(particle.state);
		}
	}

	/**
	 * perform importance calculation by matching the
	 * sensor-data against the current state
	 */
	void importance() {
		for (Particle<State>& particle : particles) {

			// reset particles probability
			particle.probability = 1.0;

			// update probability using each sensor
			for (const ParticleSensor<State>* sensor : sensors) {

				// adjust probability for the current particle using the current sensor
				double probability = sensor->getProbability(particle.state);
				particle.probability *= probability;

			}
		}
	}

	/** perform resampling */
	void resample() {
		resampler.resample(particles);
	}

	/** get a random probability between 0 and 1 */
	double getRandomProbability() {
		return (0 + rand() / (double) RAND_MAX * (1-0));
	}

	/**
	 * normalize the weight of all particles.
	 * this will ensure the sum of all weights is 1.
	 * thus, the particles hereafter resemble a probability-density-function.
	 */
	void normalize() {

		// get current sum of all probabilities
		double sum = 0;
		for (const Particle<State>& p : particles) {sum += p.probability;}

		// normalize them
		for (Particle<State>& p : particles) {p.probability = p.probability / sum;}

	}


	/** the model to use for particle modification */
	ParticleModel<State>& model;

	/** the algorithm to use for resampling */
	ParticleResampling<State>& resampler;

	/** all sensors to peform importance analysis */
	std::vector<ParticleSensor<State>*> sensors;

	/** all particles to use */
	std::vector<Particle<State>> particles;

};

#endif /* PARTICLEFILTER_H_ */
