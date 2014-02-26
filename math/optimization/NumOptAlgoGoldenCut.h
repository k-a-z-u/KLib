/*
 * NumOptAlgoGoldenCut.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGOGOLDENCUT_H_
#define K_MATH_OPT_NUMOPTALGOGOLDENCUT_H_

#include "NumOptAlgo.h"

namespace K {

	/**
	 * perform newton-approximation to find a minimum of a given function
	 *
	 * the algorithm approximates f'(x) = 0 using f'(x) and f''(x)
	 *
	 */
	class NumOptAlgoGoldenCut : public NumOptAlgo<1> {

	public:

		/** ctor */
		NumOptAlgoGoldenCut(double min = 0, double max = 1, double epsilon = 0.001) :
			min(min), max(max), epsilon(epsilon) {
			;
		}

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<1>& func, NumOptVector<1>& param) override {

			double a = min;
			double b = max;

			const double phi = (2 - (1 + sqrt(5)) / 2);

			NumOptVector<1> lambda;
			NumOptVector<1> mu;

			lambda[0] =	a + (    phi) * (b-a);
			mu[0] =		a + (1 - phi) * (b-a);

			while(true) {

				// done?
				if ((b-a) < epsilon) {break;}

				// calculate
				if (func(lambda) > func(mu)) {			// > = minimize, < = maximize
					a = lambda[0];
					lambda[0] = mu[0];
					mu[0] = a + (1 - phi) * (b-a);
				} else {
					b = mu[0];
					mu[0] = lambda[0];
					lambda[0] =	a + phi * (b-a);
				}

			}

			// done -> return center between min and max
			param[0] = (a+b) / 2.0;

		}

	private:

		double min;

		double max;

		double epsilon;

	};

}

#endif //K_MATH_OPT_NUMOPTALGOGOLDENCUT_H_
