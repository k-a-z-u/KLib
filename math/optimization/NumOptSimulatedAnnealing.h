#ifndef NUMOPTSIMULATEDANNEALING_H
#define NUMOPTSIMULATEDANNEALING_H


#include "NumOptDataVector.h"
#include <random>

namespace K {

	template <typename Scalar, int numArgs> class NumOptAlgoSimulatedAnnealing {

	private:

		/** the number of iterations to run */
		int numIterations;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;

		/** random number generator */
		std::minstd_rand0 gen;



	private:

		float probability(const float energyOld, const float energyNew, const float temperature) {
			if (energyNew < energyOld) {
				return 1.0f;
			} else {
				return std::exp( - (energyNew - energyOld) / temperature );
			}
		}

//		float energy(const NumOptDataVector<Scalar>& vec) {
//			return 1;
//		}

//		void neighbor1(NumOptDataVector<Scalar>& vec, const float temp) {
//			std::uniform_int_distribution<int> distIdx(0, vec.getSize()-1);
//			const int idx = distIdx(gen);
//			//std::normal_distribution<float> distVal(0, std::sqrt(temp));
//			//const float val = distVal(gen);
//			std::uniform_real_distribution<float> distVal(-1, +1);
//			const float val = distVal(gen);
//			vec[idx] += val;
//		}

//		void neighbor(NumOptDataVector<Scalar>& vec, const float temp) {
//			std::uniform_real_distribution<float> distVal(-1, +1);
//			for (int i = 0; i < numArgs; ++i) {
//				const float val = distVal(gen);
//				vec[i] += val;
//			}
//		}

		void neighbor(NumOptDataVector<Scalar>& vec, const float temp) {
			int x = rand() % 2;
			std::uniform_real_distribution<float> distVal(-1, +1);
			for (int i = 0; i < 3; ++i) {
				const float val = distVal(gen);
				vec[i+x*3] += val;
			}
		}

		float temperature(const int run, const int numIterations) {
			const float prog = 1.0f - ((float)run / (float)numIterations);
			return prog * 5;
		}


	public:

		/** ctor */
		NumOptAlgoSimulatedAnnealing(const int numIterations) : numIterations(numIterations) {
			;
		}





		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
			this->callback = func;
		}


		/** optimize the functions only parameter until epsilon is reached */
		template <typename Func> void calculateOptimum(Func& func, Scalar* dst) {

			std::uniform_real_distribution<float> dist01(0,1);

			// initial values
			NumOptDataVector<Scalar> cur(numArgs);
			cur = dst;
			float errCur = func(cur.constPtr());

			// new state value
			NumOptDataVector<Scalar> next(numArgs);


			// run
			for (int i = 0; i < numIterations; ++i) {

				// determine the current temperature
				const float temp = temperature(i, numIterations);

				// get a new state sample
				next = cur;
				neighbor(next, temp);

				const float errNext = func(next.constPtr());

				const float p = probability(errCur, errNext, temp);
				const float X = dist01(gen);

				// move to the new state?
				if (p >= X) {
					cur = next;
					errCur = errNext;
					if (callback) {callback(i, errCur, cur.constPtr());}
				}

			}

			// done
			cur.copyTo(dst);

		}



	};

}

#endif // NUMOPTSIMULATEDANNEALING_H
