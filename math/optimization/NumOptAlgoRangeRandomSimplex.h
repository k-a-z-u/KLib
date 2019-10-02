#ifndef NUMOPTALGORANGERANDOMSIMPLEX_H
#define NUMOPTALGORANGERANDOMSIMPLEX_H

#include "NumOptAlgoDownhillSimplex.h"

#include "NumOptAlgo.h"
#include "NumOptDataVector.h"

#include "../statistics/Statistics.h"

#include <vector>
#include <random>
#include <functional>

#include "../distribution/Normal.h"
#include "../../Assertions.h"

namespace K {

	/**
	 * start a simplex over and over at random positions
	 */
	template <typename Scalar, bool parallel = false> class NumOptAlgoRangeRandomSimplex {

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

		/** number of restarts */
		int numRetries = 50;

		/** number of refinement iterations */
		int numIterations = 50;

		/** number of parameters to optimize */
		const int numParams;

		/** range [min:max] for each parameter */
		std::vector<MinMax> valRange;

		/** random number generator */
		std::mt19937 gen;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;



	public:

		/** ctor */
		NumOptAlgoRangeRandomSimplex(const std::vector<MinMax>& range) : numParams(range.size()), valRange(range) {
			;
		}

		/** set the number of restarts to perform */
		void setNumRetries(const int num) {
			this->numRetries = num;
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

			float lastBest = 1e50;

			for (int i = 0; i < 500000; ++i) {
				NumOptDataVector<Scalar> rndParams = getRandomVector();
				const float error = func(rndParams.constPtr());
				//std::cout << "------- " << error << std::endl;

				if (error < lastBest ) {

					std::cout << ".";

					lastBest = error;

					float* startAndOut = rndParams.ptr();

					NumOptAlgoDownhillSimplex<Scalar> simplex(numParams);
					simplex.calculateOptimum(func, startAndOut);
					simplex.setNumRestarts(0);
					simplex.setMaxIterations(8);

					// adjust params if needed
					for (size_t i = 0; i < numParams; ++i) {
						MinMax& mm = valRange[i];
						if (mm.min > startAndOut[i]) {mm.min = startAndOut[i];}
						if (mm.max < startAndOut[i]) {mm.max = startAndOut[i];}
					}

					//const float error2 = func(rndParams.constPtr());
					//std::cout << "++++++ " << error << "->" << error2<< std::endl;

				}




				//func(rndParams.constPtr());

			}


//			// perform X iterations
//			for (int i = 0; i < numIterations; ++i) {

//				const float modOverIteration = 1 - (float) i / (float) numIterations;

//				// inform listeners
//				if (callback) {callback(i, population.front().error, population.front().params.constPtr());}

//				//modeRefineAll(func, population, modOverIteration);
//				modeRefineBest(func, population, modOverIteration);

//				// sort population. lowest error
//				sort(population);

//			}

//			// done. copy the best parameters
//			std::copy(&population[0].params[0], &population[0].params[numParams], dst);

		}

	private:

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


#endif // NUMOPTALGORANGERANDOMSIMPLEX_H
