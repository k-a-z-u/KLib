/*
 * NumOptAlgoDownhillSimplex.h
 *
 *  Created on: Sep 13, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_
#define K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_


#include "NumOptAlgo.h"
#include "NumOptDataVector.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace K {


	/**
	 * performs n-dimensional optimization using the
	 * downhill-simplex algorithm
	 *
	 * http://www.scholarpedia.org/article/Nelder-Mead_algorithm
	 */
	template <typename Scalar, int numArgs> class NumOptAlgoDownhillSimplex {


	using Data = NumOptDataVector<Scalar>;

	public:

		/** function parameters and function result f(param) = y */
		struct SimplexEntry {

			/** the parameters for the function to optimize */
			Data param;

			/** the result f(param) = y */
			Scalar value;

			/** ctor */
			SimplexEntry() : param(numArgs), value(0) {;}

		};

		/**
		 * ctor
		 * @param abortAt abort when difference between current worst and best value is below this value
		 * @param alpha reflection
		 * @param beta contraction
		 * @param gamma expansion
		 * @param sigma reduction
		 */
		NumOptAlgoDownhillSimplex(Scalar abortAt = 0.001, Scalar alpha = 1.0, Scalar beta = 0.5, Scalar gamma = 2.0, Scalar sigma = 0.5) :
			abortAt(abortAt), alpha(alpha), beta(beta), gamma(gamma), sigma(sigma), maxIterations(100), numRestarts(0) {
			;
		}

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
			this->callback = func;
		}

		/** optimize the functions only parameter until epsilon is reached */
		template <typename Func> void calculateOptimum(Func& func, Scalar* dst) {

			const auto& lambda = [] (const SimplexEntry& a, const SimplexEntry& b) {return a.value < b.value;};

			const int IDX_BEST = 0;
			const int IDX_WORST = numArgs;
			const int IDX_2ND_WORST = numArgs - 1;

			// we need n+1 parameter-sets during optimization
			SimplexEntry set[numArgs+1];


			// how often to refine the solution
			for (unsigned int run = 0; run <= numRestarts; ++run) {

				//std::cout << "run: " << run << std::endl;

				// init parameter set

				for (unsigned int i = 0; i < numArgs+1; ++i) {

					set[i].param = dst;				// start with the given start-vector (or 0-vector)

					// for each following entry, slightly adjust one of the n parameters
					if (i > 0) {
						if (set[i].param[i-1] == 0) {
							set[i].param[i-1] = 1.0f;
						} else {
							set[i].param[i-1] *= 1.10f;
						}
					}

				}

				// the maximum number of iterations to use
				for (unsigned int iter = 0; iter < maxIterations; ++iter) {

					// calculate f(param) = y for each of the n+1 entries
					for (unsigned int i = 0; i < numArgs+1; ++i) {
						set[i].value = func(set[i].param.constPtr());
					}

					// now sort the n+1 entries by their result (= function value) smallest error comes first
					std::sort(std::begin(set), std::end(set), lambda);

					// inform callback (if any) about the current optimum
					if (callback) {
						callback(iter, set[IDX_BEST].value, set[IDX_BEST].param.constPtr());
					}

					// done?
					if ((set[IDX_BEST].param - set[IDX_WORST].param).getLength() < abortAt) {
						break;
					}



					// -------------------------------- REFLECTION --------------------------------

					// calculate the gravity center
					// (all sets BUT the worst -> sorted: 0 to numArgs (and not numArgs+1))
					Data center(numArgs);
					for (unsigned int i = 0; i < numArgs; ++i) {center += set[i].param;}
					center /= numArgs;

					// get reflection point
					const Data reflect = center + (center - set[IDX_WORST].param) * alpha;
					const Scalar reflectValue = func(reflect.constPtr());


					if (set[IDX_BEST].value <= reflectValue && reflectValue < set[IDX_2ND_WORST].value) {

						// USE REFLECTION
						// reflect is better than second worst, but not better than current best
						set[IDX_WORST].param = reflect;
						continue;
					}

					if (reflectValue < set[IDX_BEST].value) {

						// USE EXPANSION
						// reflect is better than the current best

						const Data expand = center + (reflect - center) * gamma;
						const Scalar expandValue = func(expand.constPtr());

						// is expansion better than reflection?
						if (expandValue < reflectValue) {
							set[IDX_WORST].param = expand;
							continue;
						} else {
							set[IDX_WORST].param = reflect;
							continue;
						}

					}

					if (reflectValue >= set[IDX_2ND_WORST].value) {

						// CONTRACTION
						// reflection is worst than second worst
						//std::cout << "contraction" << std::endl;

						if (set[IDX_2ND_WORST].value <= reflectValue && reflectValue < set[IDX_WORST].value) {

							const Data contract = center + (reflect - center) * beta;
							const Scalar contractValue = func(contract.constPtr());

							// contraction better than worst? -> replace worst
							if (contractValue <= reflectValue) {
								set[IDX_WORST].param = contract;
								continue;
							}

						} else {

							const Data contract = center + (set[IDX_WORST].param - center) * beta;
							const Scalar contractValue = func(contract.constPtr());

							// contraction better than worst? -> replace worst
							if (contractValue < set[IDX_WORST].value) {
								set[IDX_WORST].param = contract;
								continue;
							}

						}

					}



					// REDUCTION
					// 'resize' the simplex
					for (unsigned int i = 1; i < numArgs+1; ++i) {
						set[i].param = set[IDX_BEST].param + (set[i].param - set[IDX_BEST].param) * sigma;
					}



				}

				// the best result
				set[IDX_BEST].param.copyTo(dst);
				//param = set[IDX_BEST].param;


			}

		}

		/** abort after the given number of iterations */
		void setMaxIterations(unsigned int max) {
			this->maxIterations = max;
		}

		/** how often to restart the algorithm after having found a valid solution (default: 0) */
		void setNumRestarts(unsigned int cnt) {
			this->numRestarts = cnt;
		}

	private:

		/** when to abort the algorithm */
		Scalar abortAt;

		/** the reflection factor */
		Scalar alpha;

		/** the contraction factor */
		Scalar beta;

		/** the expansion factor */
		Scalar gamma;

		/** the reduction factor */
		Scalar sigma;

		/** stop after the given number of iterations */
		unsigned int maxIterations;

		/** how often to restart the algorithm after having found a valid solution */
		unsigned int numRestarts;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* params)> callback;


	};

}


#endif /* K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_ */
