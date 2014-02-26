/*
 * NumOptAlgoGradient.h
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTALGOGRADIENT_H_
#define K_MATH_OPT_NUMOPTALGOGRADIENT_H_

#include "NumOptVector.h"
#include "NumOptAlgoNewton.h"

namespace K {

	#define NumOptGradDumpVec(vec) {\
			std::cout << "[";\
			for (int i = 0; i < numArgs; ++i) {std::cout << vec[i] << ",";}\
			std::cout << "]" << std::endl;\
	}


	//#define NOAG_VERB


	#ifdef NOAG_VERB
	#include <iostream>
	#endif

	/**
	 * perform numerical optimization using gradients
	 */
	template <int numArgs> class NumOptAlgoGradient : public NumOptAlgo<numArgs> {



	public:

		/**
		 * ctor
		 * @param func the function to optimize
		 * @param lambdaFunc a function to optimize 1 parameter (used for gradient's lambda value)
		 * @param delta the step size to use
		 * @param abortAt abort when result's "error" is less than this
		 */
		NumOptAlgoGradient(NumOptAlgo<1>& lambdaOpt, double delta = 0.00001, double abortAt = 0.01) :
			lambdaOpt(lambdaOpt), delta(delta), abortAt(abortAt), maxIterations(5000) {;}

		/**
		 * calculate the optimum starting at the given vector and
		 * storing the result into the vector after calculation.
		 * @param dst the start point and result destination
		 */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& dst) override {

			for (unsigned int cnt = 0; cnt < maxIterations; ++cnt) {

	#ifdef NOAG_VERB
				std::cout << "starting at: " << std::endl;
				NumOptGradDumpVec(dst);
	#endif

				NumOptVector<numArgs> gradient;

				// calculate current gradient
				func.getGradient(dst, gradient, delta);

	#ifdef NOAG_VERB
				std::cout << "gradient: ";
				NumOptGradDumpVec(gradient);
	#endif

				// abort?
				if (gradient.getLength() < abortAt) {
					//std::cout << "done!" << std::endl;
					return;
				}

				// calculate lambda value for gradient
				double lambda = calcGradientLambda(func, dst, gradient);
				//std::cout << "best lambda for gradient: " << lambda << std::endl;

				// TODO
				// another abortion criteria?
				if ((gradient * lambda).getLength() < abortAt) {
					return;
				}

				// calculate next iteration's starting vector
				dst -= gradient * lambda;

				//std::cout << "cur f(x) = " << func.getValue(dst) << std::endl;

			}

		}

		/** abort after the given number of iterations */
		void setMaxIterations(unsigned int max) {
			this->maxIterations = max;
		}

	private:

		/** the function to optimize the gradient's lambda */
		NumOptAlgo<1>& lambdaOpt;

		/** the delta to use for gradient calculation */
		double delta;

		/** stop if gradient's length is smaller than this value */
		double abortAt;

		/** stop after the given number of iterations */
		unsigned int maxIterations;

		/**
		 * the function to optimize the lambda value for the gradient
		 * another opt-algorithm (e.g. newton) will be used to solve this problem
		 */
		template <int parentArgs> struct LambdaOptFunc : public NumOptFunction<1> {

			const NumOptVector<parentArgs>& curArgs;
			const NumOptVector<parentArgs>& gradient;
			NumOptFunction<parentArgs>& optFunc;

			/** ctor */
			LambdaOptFunc(const NumOptVector<parentArgs>& curArgs, const NumOptVector<parentArgs>& gradient, NumOptFunction<parentArgs>& optFunc) :
				curArgs(curArgs), gradient(gradient), optFunc(optFunc) {;}

			/** calc */
			double getValue(const NumOptVector<1>& lambda) const {
				return optFunc(curArgs - (gradient * lambda[0]));
			}

		};

		/** get gradient lambda */
		double calcGradientLambda(NumOptFunction<numArgs>& func, const NumOptVector<numArgs>& curArgs, const NumOptVector<numArgs>& gradient) {

			// calculate lambda for gradient using newton approximation
			//std::array<double, 1> s = {1};
			NumOptVector<1> start;
			start[0] = 1;
			LambdaOptFunc<numArgs> f(curArgs, gradient, func);
			lambdaOpt.calculateOptimum(f, start);
			return start[0];

		}


	};

}

#endif /* K_MATH_OPT_NUMOPTALGOGRADIENT_H_ */
