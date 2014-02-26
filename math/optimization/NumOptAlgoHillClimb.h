/*
 * NumOptAlgoHillClimb.h
 *
 *  Created on: Sep 5, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGOHILLCLIMB_H_
#define K_MATH_OPT_NUMOPTALGOHILLCLIMB_H_

#include "NumOptAlgo.h"

namespace K {

	/**
	 * performs numerical optimization using hill-climbing
	 * (changing each individual dimension into both possible directions)
	 *
	 */
	template <int numArgs, bool useRandom = false> class NumOptAlgoHillClimb : public NumOptAlgo<numArgs> {

	public:

		/** ctor */
		NumOptAlgoHillClimb(double stepSize = 1) :
			stepSize(stepSize) {
			;
		}

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& param) override {

			func.getValue(param);

			// initialize neighbors-object once
			std::vector<NumOptVector<numArgs>> neighbors;
			neighbors.resize(numArgs * 4);

			// start using the given start-vector
			double bestFX = func(param);

			// perform iterations
			for (unsigned int i = 0; i < 1024 * 128; ++i) {

				// get all neighbors around the current best value
				getNeigbors(param, neighbors);

				// evaluate these neighbors
				bool foundBetterParams = false;
				for (unsigned int j = 0; j < (4*numArgs); ++j) {

					// the neighbor
					const NumOptVector<numArgs>& neighbor = neighbors[j];

					// get f(x) for the neighbor
					double fx = func(neighbor);

					// better?
					if (fx < bestFX) {
						bestFX = fx;
						param = neighbor;
						foundBetterParams = true;
					}

				}

				// abort?
				if (!foundBetterParams) {return;}

			}

		}
		/** calculate neighbor list */
		void getNeigbors(const NumOptVector<numArgs>& curArgs, std::vector<NumOptVector<numArgs>>& neighbors) {

			// create neighbors for each individual dimension
			for (unsigned int i = 0; i < numArgs; ++i) {

				// create four copies
				neighbors[i*4 + 0] = curArgs;
				neighbors[i*4 + 1] = curArgs;
				neighbors[i*4 + 2] = curArgs;
				neighbors[i*4 + 3] = curArgs;

				// proceed to the left and to the right using 2 step sizes
				neighbors[i*4 + 0][i] += stepSize;
				neighbors[i*4 + 1][i] += stepSize / 25.0;
				neighbors[i*4 + 2][i] -= stepSize / 25.0;
				neighbors[i*4 + 3][i] -= stepSize;

				// add some random values to (hopefully) speed things up
				if (useRandom) {
					neighbors[i*4 + 0][i] += getRand(-stepSize/5, stepSize/5);
					neighbors[i*4 + 3][i] += getRand(-stepSize/5, stepSize/5);
				}

			}

		}

		double getRand(double min, double max) {
			double rnd = rand() / (double) RAND_MAX;
			double range = max - min;
			return (rnd*range) + min;
		}


	private:

		double stepSize;

	};


}


#endif /* K_MATH_OPT_NUMOPTALGOHILLCLIMB_H_ */
