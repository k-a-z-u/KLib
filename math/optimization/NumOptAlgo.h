/*
 * NumOptAlgo.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGO_H_
#define K_MATH_OPT_NUMOPTALGO_H_

#include "NumOptVector.h"
#include "NumOptFunction.h"

namespace K {

	/**
	 * interface for all algorithms providing numerical optimization
	 */
	template <int numArgs> class NumOptAlgo {

	public:

		/** dtor */
		virtual ~NumOptAlgo() {;}

		/**
		 * calculate the optimum and store it into dst.
		 * dst might be used as a starting combination depending on the underlying algorithm
		 */
		virtual void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& dst) = 0;

	};

}

#endif /* K_MATH_OPT_NUMOPTALGO_H_ */
