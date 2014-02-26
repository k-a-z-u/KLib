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

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& param) override {

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
						set[i].param[i-1] += 1.1;
					} else {
						set[i].param[i-1] *= 1.1;
					}

				}

				// the maximum number of iterations to use
				unsigned int iter;
				for (iter = 0; iter < maxIterations; ++iter) {

					// calculate f(param) = y for each of the n+1 entries
					for (unsigned int i = 0; i < numArgs+1; ++i) {
						set[i].value = func(set[i].param);
					}

					// sort n+1 entries by their result
					// set[0] = minimum
					// set[n] = maximum
					const auto& lambda = [] (const SimplexEntry& a, const SimplexEntry& b) {return a.value < b.value;};
					std::sort(std::begin(set), std::end(set), lambda);

					//			for (unsigned int i = 0; i < numArgs+1; ++i) {
					//				std::cout << "params:\t" << set[i].param << "\t-> " << set[i].value << std::endl;
					//			}

					// -------------------------------- REFLECTION --------------------------------

					// calculate the gravity center
					// (all points BUT the worst -> numArgs (and not numArgs+1))
					NumOptVector<numArgs> center;
					for (unsigned int i = 0; i < numArgs; ++i) {center += set[i].param;}
					center /= numArgs;

					// get reflection point
					NumOptVector<numArgs> reflect = set[0].param + (set[0].param - set[IDX_WORST].param) * alpha;
					double reflectValue = func(reflect);


					if (set[0].value <= reflectValue && reflectValue < set[IDX_2ND_WORST].value) {

						// REFLECTION
						// reflect is better than second worst, but not better than current best
						//std::cout << "reflection" << std::endl;

						set[IDX_WORST].param = reflect;

					} else if (reflectValue < set[0].value) {

						// EXPANSION
						// reflect is better than the current best
						//std::cout << "expansion" << std::endl;

						NumOptVector<numArgs> expand = set[0].param + (set[0].param - set[IDX_WORST].param) * gamma;
						double expandValue = func(expand);

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

						NumOptVector<numArgs> contract = set[0].param + (set[0].param - set[IDX_WORST].param) * rho;
						double contractValue = func(contract);

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

					//std::cout << "center:\t" << center << std::endl;
					//std::cout << "ref-p:\t" << ref << "\t-> " << value << std::endl;
					//std::cout << "------------------------------------------" << std::endl;

					// done?
					if ((set[IDX_BEST].param - set[IDX_WORST].param).getLength() < abortAt) {
						break;
					}

				}

				//std::cout << std::endl << std::endl<<iter << std::endl;
				// the best result
				param = set[IDX_BEST].param;
				//std::cout << "best: " << set[IDX_BEST].param << std::endl;

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


	};

}


#endif /* K_MATH_OPT_NUMOPTALGODOWNHILLSIMPLEX_H_ */
