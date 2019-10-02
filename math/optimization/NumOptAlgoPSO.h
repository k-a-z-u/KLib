#ifndef NUMOPTALGOPSO_H
#define NUMOPTALGOPSO_H

#include "NumOptAlgo.h"

#include <vector>
#include <random>
#include <functional>
#include <limits>

#include "../distribution/Normal.h"

namespace K {

	/**
	 * particle swarm optimization
	 * https://www.youtube.com/watch?v=JhgDMAm-imI
	 */
	template <typename Scalar, bool parallel = false> class NumOptAlgoPSO {

		using Params = std::vector<Scalar>;

	public:

		/** callback-function to inform after every run */
		using Callback = std::function<void(const int iteration, const Scalar result, const Scalar* values)>;

	private:

		// global, yet detected best parameter set
		Params globalyBestParams;
		Scalar globalBestVal;

		struct Entry {

			// personal, yet detected best parameter set
			Params personalBestParams;
			Scalar personalBestVal;

			// personal, current parameter set
			Params curParams;
			Scalar curVal;

		};

		std::vector<Entry> population;
		std::vector<std::uniform_real_distribution<Scalar>> dist;

	private:

		/** number of parameters */
		size_t numParams;

		/** the size of the population (number of elements) */
		size_t populationSize = 100;

		/** the maximum number of iterations */
		size_t maxIterations = 50;

		std::mt19937 gen;

		Callback callback;


	public:

		/** ctor */
		NumOptAlgoPSO(const size_t numParams) : numParams(numParams) {

			gen.seed(1234);

			dist.resize(numParams);
			for (size_t i = 0; i < numParams; ++i) {
				dist[i] = std::uniform_real_distribution<Scalar>(-5, +5);
			}

		}

		/** set a callback-function to inform after every run */
		void setCallback(Callback func) {
			this->callback = func;
		}

		template <typename Func> void calculateMinimum(Func& func, Scalar* dst) {

			// reset
			globalBestVal = std::numeric_limits<Scalar>::max();
			initWithZeros(globalyBestParams);

			// initalize population
			population.resize(populationSize);
			for (size_t i = 0; i < populationSize; ++i) {
				initWithRandomValues(population[i].curParams, 1);
				initWithRandomValues(population[i].personalBestParams, 1);
				population[i].personalBestVal = std::numeric_limits<float>::max();
			}

			Scalar strength = 1.0;

			for (size_t run = 0; run < maxIterations; ++run) {

				strength *= 0.995;

				// update
				//const Scalar s = 0.33f;
				for (Entry& e : population) {
					Params next;
					initWithRandomValues(next, 0.1f * strength);
					addTo(next, e.curParams,				1.1f * strength);
					moveTowards(next, e.personalBestParams,	0.1f * strength);
					moveTowards(next, globalyBestParams,	0.1f * strength);
					e.curParams = next;
				}

				// weight
				for (Entry& e : population) {

					// get result
					const Scalar* values = e.curParams.data();
					const Scalar res = func(values);

					// new personal minimum?
					if (res < e.personalBestVal) {
						e.personalBestVal = res;
						e.personalBestParams = e.curParams;
					}

					// new global minimum?
					if (res < globalBestVal) {
						globalBestVal = res;
						globalyBestParams = e.curParams;
					}

				}

				// inform callback (if any) about the current optimum
				if (callback) {
					callback(run, globalBestVal, globalyBestParams.data());
				}

			}

			// done
			std::copy(globalyBestParams.begin(), globalyBestParams.end(), dst);

		}

	private:

		void initWithRandomValues(Params& p, Scalar mul) {
			p.resize(numParams);
			for (size_t i = 0; i < numParams; ++i) {
				p[i] = dist[i](gen) * mul;
			}
		}

		void initWithZeros(Params& p) {
			p.resize(numParams);
			for (size_t i = 0; i < numParams; ++i) {
				p[i] = 0;
			}
		}

		void addTo(Params& dst, const Params& src, const Scalar impact) {
			for (size_t i = 0; i < numParams; ++i) {
				dst[i] += src[i] * impact;
			}
		}

		void moveTowards(Params& dst, const Params& src, const Scalar impact) {
			for (size_t i = 0; i < numParams; ++i) {
				dst[i] += (src[i]-dst[i]) * impact;
			}
		}

	};

}

#endif // NUMOPTALGOPSO_H
