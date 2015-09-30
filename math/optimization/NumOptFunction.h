/*
 * NumOptFunction.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTFUNCTION_H_
#define K_MATH_OPT_NUMOPTFUNCTION_H_

#include "../../Assertions.h"
#include "NumOptVector.h"

namespace K {

	/** the delta to use for derivations */
	#define NumOptFunctionDelta		0.00001

	template <int numArgs> class NumOptFunction {


	public:

		/** dtor */
		virtual ~NumOptFunction() {;}

		/** get function's value at the given position */
		virtual double getValue(const NumOptVector<numArgs>& args) const = 0;

		/** get function's value at the given position */
		double operator () (const NumOptVector<numArgs>& args) {
			double val = getValue(args);
			assert(val == val);			// catch NaN
			return val;
		}

		/** get 1st derivation at x f'(x) */
		double getF1(const NumOptVector<numArgs>& x, double delta = 0.00001) const {
			double v1 = getValue(x + delta/2);
			double v2 = getValue(x - delta/2);
			return ( v1 - v2 ) / delta;
		}

		/** get 2nd erivation at x f''(x) */
		double getF2(const NumOptVector<numArgs>& x, double delta = 0.00001) const {
			double v1 = getF1(x + delta/2, delta);
			double v2 = getF1(x - delta/2, delta);
			return ( v1 - v2 ) / delta;
		}

		/** get the functions gradient (partial derivative) at a given position */
		void getGradient(const NumOptVector<numArgs>& x, NumOptVector<numArgs>& gradient, double delta = 0.00001) {

			// temporal copy of the function arguments "x"
			NumOptVector<numArgs> tmp = x;

			// calculate gradient (partial derivative)
			for (unsigned int i = 0; i < numArgs; ++i) {

				// calculate at slight offset f(x + delta)
				tmp[i] = x[i] + delta;
				const double v1 = getValue(tmp);
				_assertNotNAN(v1, "to-be-optimized function returned NaN");

				// calculate original f(x)
				tmp[i] = x[i];
				const double v2 = getValue(tmp);
				_assertNotNAN(v1, "to-be-optimized function returned NaN");

				// get gradient component
				gradient[i] = (v1 - v2) / delta;

			}

		}

	};

}

#endif /* K_MATH_OPT_NUMOPTFUNCTION_H_ */
