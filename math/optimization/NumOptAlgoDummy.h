/*
 * NumOptAlgoDummy.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGODUMMY_H_
#define K_MATH_OPT_NUMOPTALGODUMMY_H_

#include "NumOptAlgo.h"

namespace K {

	/**
	 * dummy 1d optimizer
	 * always returns the same value
	 */
	class NumOptAlgoDummy : public NumOptAlgo<1> {

	public:

		/** ctor */
		NumOptAlgoDummy(double value) : value(value) {;}

		/** optimize the functions only parameter until epsilon is reached */
		void calculateOptimum(NumOptFunction<1>& func, NumOptVector<1>& param) override {
			(void) func;
			param[0] = value;
		}

	private:

		/** the value to return as opt-result */
		double value;

	};

}

#endif /* K_MATH_OPT_NUMOPTALGODUMMY_H_ */
