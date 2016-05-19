#ifndef K_MATH_OPT_NUMOPTALGOSIMPLE_H
#define K_MATH_OPT_NUMOPTALGOSIMPLE_H



#include "NumOptAlgo.h"
#include "NumOptDataVector.h"

#include "../statistics/Statistics.h"

#include <vector>
#include <random>
#include <functional>

#include "../distribution/Normal.h"

namespace K {

	template <typename Scalar> class NumOptAlgoSimple {

	private:

		/** number of parameters */
		int numParams;

		/** the maximum number of iterations */
		int maxIterations = 500;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;

	public:

		/** ctor */
		NumOptAlgoSimple(const int numParams) : numParams(numParams) {
			;
		}

		/** abort after the given number of iterations */
		void setMaxIterations(const unsigned int max) {
			this->maxIterations = max;
		}

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
			this->callback = func;
		}

		/**
		 * how it works:
		 */
		template <typename Func> void calculateOptimum(Func& func, Scalar* dst) {

			const Scalar delta = 0.01;
			Scalar gradients[numParams];
			NumOptDataVector<Scalar> cur(numParams); cur = dst;

			for (int i = 0; i < maxIterations; ++i) {

				volatile Scalar baseError = func(cur.constPtr());

				// raise callback
				if (callback) {callback(i, baseError, cur.constPtr());}

				// get gradients
				for (int dim = 0; dim < numParams; ++dim) {
					NumOptDataVector<Scalar> vec = cur;
					vec[dim] += delta;
					Scalar error = func(vec.constPtr());
					gradients[dim] = (error - baseError) / delta;
				}

				// find best
				auto it = std::min_element(&gradients[0], &gradients[numParams]);
				int bestIdx = it - &gradients[0];

				// follow the best one
				//Scalar bestGrad = gradients[bestIdx];

				again:
				cur[bestIdx] += delta;
				Scalar newError = func(cur.constPtr());
				if (newError < baseError) {baseError = newError; goto again;}

			}


		}


	};

}

#endif // K_MATH_OPT_NUMOPTALGOSIMPLE_H
