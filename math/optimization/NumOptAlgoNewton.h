/*
 * NumOptAlgoNewton.h
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGONEWTON_H_
#define K_MATH_OPT_NUMOPTALGONEWTON_H_


#include "NumOptAlgo.h"
#include <cassert>

namespace K {

	/**
	 * perform newton-approximation to find a minimum of a given function
	 *
	 * the algorithm approximates f'(x) = 0 using f'(x) and f''(x)
	 *
	 */
	class NumOptAlgoNewton : public NumOptAlgo<1> {

	public:

		/** ctor */
		NumOptAlgoNewton(double epsilon = 0.001) : epsilon(epsilon) {;}

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<1>& func, NumOptVector<1>& param) override {

			// TODO is it ok to always start at 1??
			//std::array<double,1> s = {1};
			NumOptVector<1> cur;
			cur[0] = 1;

			NumOptVector<1> next;

			// optimize until |cur - next| < epsilon
			while(true) {

				// calculate next value
				double f1 = func.getF1(cur);
				double f2 = func.getF2(cur);

				//std::cout << "f'(x) = " << f1 << "  f''(x) = " << f2 << " div = " << (f1/f2) << std::endl;

				next[0] = cur[0] - (f1 / f2);

				// done?
				double delta = fabs(cur[0] - next[0]);
				if (delta < epsilon) {break;}

				// proceed
				cur[0] = next[0];

				// sanity check
				assert(cur[0] == cur[0]);

			}

			// done -> return
			param[0] = next[0];

		}

	private:

		double epsilon;

	};

}

#endif /* K_MATH_OPT_NUMOPTALGONEWTON_H_ */
