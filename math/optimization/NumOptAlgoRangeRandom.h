#ifndef K_MATH_OPT_NUMOPTALGORANGERANDOM_H
#define K_MATH_OPT_NUMOPTALGORANGERANDOM_H


#include "NumOptAlgo.h"
#include "NumOptDataVector.h"

#include "../statistics/Statistics.h"

#include <vector>
#include <random>
#include <functional>

#include "../distribution/Normal.h"
#include "../../Assertions.h"

namespace K {

	template <typename Scalar> class NumOptAlgoRangeRandom {

	public:
		struct MinMax {
			Scalar min;
			Scalar max;
			MinMax(const Scalar min, const Scalar max) : min(min), max(max) {
				_assertTrue(min <= max, "min must be <= max!");
			}
			const Scalar diff() const {return max - min;}
		};

	private:

		/** one entity within the population */
		struct Entity {
			NumOptDataVector<Scalar> params;
			Scalar error;
			Entity(const NumOptDataVector<Scalar>& params, const float error) : params(params), error(error) {;}
		};


	private:

		/** number of samples per iteration */
		int populationSize = 100;

		/** number of refinement iterations */
		int numIterations = 50;

		/** number of parameters to optimize */
		const int numParams;

		/** range [min:max] for each parameter */
		const std::vector<MinMax> valRange;
		std::vector<Scalar> valDeviation;

		/** random number generator */
		std::minstd_rand gen;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;



	public:

		/** ctor */
		NumOptAlgoRangeRandom(const std::vector<MinMax>& range) : numParams(range.size()), valRange(range) {

			// allowed deviation for each parameter
			// starts at 1/10 of max - min
			for (const MinMax& mm : valRange) {
				valDeviation.push_back( mm.diff() / 10 );
			}

		}

		/** set the size of the population  (samples per iteration) */
		void setPopulationSize(const int num) {
			this->populationSize = num;
		}


		/** set the number of iterations to perform */
		void setNumIerations(const int num) {
			this->numIterations = num;
		}



		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
			this->callback = func;
		}


		/** optimize the functions only parameter until epsilon is reached */
		template <typename Func> void calculateOptimum(Func& func, Scalar* dst) {

			// create the initial distribution
			std::vector<Entity> population;
			for (int i = 0; i < populationSize; ++i) {
				const NumOptDataVector<Scalar> rndParams = getRandomVector();
				const float error = func(rndParams.constPtr());
				population.push_back( Entity(rndParams, error) );
			}

			// sort population. highest fitness comes first
			sort(population);



			// perform X iterations
			for (int i = 0; i < numIterations; ++i) {

				const float modOverIteration = 1 - (float) i / (float) numIterations;

				// inform listeners
				if (callback) {callback(i, population.front().error, population.front().params.constPtr());}

				//modeRefineAll(func, population, modOverIteration);
				modeRefineBest(func, population, modOverIteration);

				// sort population. lowest error
				sort(population);

			}

			// done. copy the best parameters
			std::copy(&population[0].params[0], &population[0].params[numParams], dst);

		}

	private:

		/** keep the first X elements as-is. refine all others (higher error -> higher modification) */
		template <typename Func> inline void modeRefineAll(Func& func, std::vector<Entity>& population, const float modOverIteration) {

			const int skip = 10;

			for (int x = skip; x < populationSize; ++x) {

				const float modOverPosition = (float) x / (float) populationSize;

				const float modStrength = modOverPosition * modOverIteration;

				const	Entity& src = population[x - skip];
						Entity& dst = population[x];

				modify(src.params, dst.params, valDeviation, modStrength);
				dst.error = func(dst.params.constPtr());

			}

		}

		/** keep the best X elements and let the rest be a "refinement" of the best ones */
		template <typename Func> inline void modeRefineBest(Func& func, std::vector<Entity>& population, const float modOverIteration) {

			// how many source elements to use for the next population
			// too low -> might miss other good ones!
			// too high -> needs more iterations
			const float baseSize = 0.25f;
			const int numBase = populationSize * baseSize;	// the values to refine

			// distribution to pick source elements [= good ones]
			// favor lower indicies (as those have a lower error)
			// output range ~[0:10] where near-0 is most likely
			std::gamma_distribution<float> dist(1,2);

			for (int dstIdx = numBase; dstIdx < populationSize; ++dstIdx) {

				// gamma[1,2] outputs ~[0:10]
				// to get srcIdx between [0:numBase] we have to divide by 10
				const int srcIdx = dist(gen) * numBase / 10;

				// modification strength is decreased within increasing number of iterations (cooling)
				const float modStrength = modOverIteration;

				const	Entity& src = population[srcIdx];
						Entity& dst = population[dstIdx];

				modify(src.params, dst.params, modStrength);
				dst.error = func(dst.params.constPtr());

			}

		}



		/** sort population by error (lower ones come first) */
		inline void sort(std::vector<Entity>& population) {
			static auto lambda = [] (const Entity& e1, const Entity& e2) {return e1.error < e2.error;};
			std::sort(population.begin(), population.end(), lambda);
		}

		/** modify the given entity using the allowed parameter deviation and modification strength */
		void modify(const NumOptDataVector<Scalar>& src, NumOptDataVector<Scalar>& dst, const float modStrength) {

			// update each param by drawing a random value within a certain range
			for (int p = 0; p < numParams; ++p) {
				//again:;
				std::uniform_real_distribution<float> dist( -valDeviation[p], +valDeviation[p] );
				const float mod = dist(gen) * modStrength;
				const float next = src[p] + mod;
				//if (next < valRange[p].min) {goto again;}
				//if (next > valRange[p].max) {goto again;}
				dst[p] = next;
			}

		}

		/** create a new random population-member that respects the range [min:max] for each parameter */
		NumOptDataVector<Scalar> getRandomVector() {

			NumOptDataVector<Scalar> vec(numParams);

			// draw one random value within [min:max] for each parameter
			for (int p = 0; p < numParams; ++p) {
				std::uniform_real_distribution<Scalar> dist(valRange[p].min, valRange[p].max);
				const Scalar val = dist(gen);
				vec[p] = val;
			}

			return vec;

		}


	};

}


#endif // K_MATH_OPT_NUMOPTALGORANGERANDOM_H
