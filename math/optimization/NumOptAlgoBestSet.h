#ifndef NUMOPTALGOBESTSET_H
#define NUMOPTALGOBESTSET_H

#include "NumOptDataVector.h"
#include <random>

namespace K {

	template <typename Scalar, int numArgs> class NumOptAlgoBestSet {

	private:

		struct Entry {
			NumOptDataVector<Scalar> vec;
			float err;
			Entry(const NumOptDataVector<Scalar>& vec, const float err) : vec(vec), err(err) {;}
			bool operator < (const Entry& e) const {return err < e.err;}
		};

		std::vector<Entry> entries;

		int setSize1 = 25;
		int setSize2 = 1;
		int numIterations = 200;
		std::minstd_rand0 gen;

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;


	public:

		/** ctor */
		NumOptAlgoBestSet(const int numIterations) : numIterations(numIterations) {
			;
		}

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
			this->callback = func;
		}


		/**
		 * during the modification step, adjust all parameters randomly
		 * in the beginning, the adjustments are usually strong ("start")
		 * in the end, the adjustemnts are usually weakter ("end")
		 */
		class ModifyAll {
		private:
			float start;
			float end;
			std::minstd_rand0 gen;
		public:
			ModifyAll(const float start, const float end) : start(start), end(end) {;}
			void operator() (const float progress, NumOptDataVector<Scalar>& vec) {
				const float s = start + ((end-start) * progress);
				std::uniform_real_distribution<float> dist(-s, +s);
				for (int i = 0; i < numArgs; ++i) { vec[i] += dist(gen); }
			}
		};

		/**
		 * try to find a minimum within the given function
		 * during each iteration, the current value-set is changed
		 * using the given modifier. this one is problem dependent!
		 * however, some default modifiers exist within this class
		 */
		template <typename FuncErr, typename Modifier> void findMinimum(FuncErr& errFunc, Modifier modifier, Scalar* dst) {

			// start with the given parameter-set
			float startErr = errFunc(dst);
			for (int i = 0; i < setSize1; ++i) {
				const Entry e(NumOptDataVector<Scalar>(numArgs, dst), startErr);
				entries.push_back(e);
				startErr += 1;
			}

			// run X iterations
			for (int i = 0; i < numIterations; ++i) {

				// current progress [0:1]
				const float progress = ((float)i / (float)numIterations);
				const int size = setSize1 + ((setSize2-setSize1) * progress);

				// pick a random entry to modify
				const int idx = (int) (rand() % size);

				// copy it and modify the copy
				Entry copy = entries[idx];
				modifier(progress, copy.vec);

				// determine the copy's error
				copy.err = errFunc(copy.vec.constPtr());

				// better than the worst one? -> insert!
				if (copy.err < entries.back().err) {
					const bool isBest = copy.err < entries.front().err;
					insertSorted(entries, copy);
					if (callback && isBest) {callback(i, entries[0].err, entries[0].vec.constPtr());}
				}

				while (entries.size() > size) {entries.erase(entries.end()-1);}

			}

			// done
			entries[0].vec.copyTo(dst);

		}

	private:

		/** helper for insertion into sorted vector */
		template <typename T> typename std::vector<T>::iterator insertSorted( std::vector<T> & vec, T const& item ) {
			return vec.insert( std::upper_bound( vec.begin(), vec.end(), item ), item );
		}

	};

}

#endif // NUMOPTALGOBESTSET_H
