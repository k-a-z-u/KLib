/*
 * NumOptAlgoDownhillSimplex.h
 *
 *  Created on: Sep 13, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_
#define K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_


#include "NumOptAlgo.h"
#include <algorithm>
#include <functional>

namespace K {

	/**
	 * performs n-dimensional optimization using the
	 * downhill-simplex algorithm
	 */
	template <int numArgs> class NumOptAlgoDownhillSimplex : public NumOptAlgo<numArgs> {

	public:

		/** function parameters and function result f(param) = y */
		struct SimplexEntry {

			/** the parameters for the function to optimize */
			NumOptVector<numArgs> param;

			/** the result f(param) = y */
			double value;

		};

		/**
		 * ctor
		 * @param abortAt abort when difference between current worst and best value is below this value
		 * @param alpha reflection
		 * @param rho contraction
		 * @param gamma expansion
		 * @param sigma reduction
		 */
		NumOptAlgoDownhillSimplex(double abortAt = 0.001, double alpha = 1.0, double rho = -0.5, double gamma = 2.0, double sigma = 0.5) :
			abortAt(abortAt), alpha(alpha), rho(rho), gamma(gamma), sigma(sigma), maxIterations(5000), numRestarts(0) {
			;
		}

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const float error, const NumOptVector<numArgs>&)> func) {
			this->callback = func;
		}

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& param) override {

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
				set[0].param = param;				// first value = given start-vector (or 0-vector)
				for (unsigned int i = 1; i < numArgs+1; ++i) {

					// for each following entry, slightly adjust one of the n parameters
					if (set[i].param[i-1] == 0) {
						set[i].param[i-1] = 1.0;
					} else {
						set[i].param[i-1] *= 1.10;
					}

				}

				// the maximum number of iterations to use
				for (unsigned int iter = 0; iter < maxIterations; ++iter) {

					// calculate f(param) = y for each of the n+1 entries
					for (unsigned int i = 0; i < numArgs+1; ++i) {
						set[i].value = func(set[i].param);
					}

					// now sort the n+1 entries by their result (= function value)
					std::sort(std::begin(set), std::end(set), lambda);


					// -------------------------------- REFLECTION --------------------------------

					// calculate the gravity center
					// (all points BUT the worst -> numArgs (and not numArgs+1))
					NumOptVector<numArgs> center;
					for (unsigned int i = 0; i < numArgs; ++i) {center += set[i].param;}
					center /= numArgs;

					// get reflection point
					const NumOptVector<numArgs> reflect = set[0].param + (set[0].param - set[IDX_WORST].param) * alpha;
					const double reflectValue = func(reflect);


					if (set[0].value <= reflectValue && reflectValue < set[IDX_2ND_WORST].value) {

						// REFLECTION
						// reflect is better than second worst, but not better than current best
						//std::cout << "reflection" << std::endl;

						set[IDX_WORST].param = reflect;

					} else if (reflectValue < set[0].value) {

						// EXPANSION
						// reflect is better than the current best
						//std::cout << "expansion" << std::endl;

						const NumOptVector<numArgs> expand = set[0].param + (set[0].param - set[IDX_WORST].param) * gamma;
						const double expandValue = func(expand);

						// is expansion better than reflection?
						if (expandValue < reflectValue) {
							set[IDX_WORST].param = expand;
						} else {
							set[IDX_WORST].param = reflect;
						}

					} else {

						// CONTRACTION
						// reflection is worst than second worst
						//std::cout << "contraction" << std::endl;

						const NumOptVector<numArgs> contract = set[0].param + (set[0].param - set[IDX_WORST].param) * rho;
						const double contractValue = func(contract);

						// contraction better than worst? -> replace worst
						if (contractValue < set[IDX_WORST].value) {
							set[IDX_WORST].param = contract;
						} else {

							// REDUCTION
							// 'resize' the simplex
							// std::cout << "reduction" << std::endl;

							for (unsigned int i = 1; i < numArgs+1; ++i) {
								set[i].param = set[IDX_BEST].param + (set[i].param - set[IDX_BEST].param) * sigma;
							}

						}

					}

					// inform callback (if any) about the current optimum
					if (callback) {
						callback(iter, set[IDX_BEST].value, set[IDX_BEST].param);
					}

					// done?
					if ((set[IDX_BEST].param - set[IDX_WORST].param).getLength() < abortAt) {
						break;
					}

				}

				// the best result
				param = set[IDX_BEST].param;

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
		double abortAt;

		/** the reflection factor */
		double alpha;

		/** the contraction factor */
		double rho;

		/** the expansion factor */
		double gamma;

		/** the reduction factor */
		double sigma;

		/** stop after the given number of iterations */
		unsigned int maxIterations;

		/** how often to restart the algorithm after having found a valid solution */
		unsigned int numRestarts;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const float error, const NumOptVector<numArgs>&)> callback;


	};

}


#endif /* K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_ */
