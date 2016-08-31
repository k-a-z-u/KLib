#ifndef K_MATH_MEDIAN_H
#define K_MATH_MEDIAN_H

#include <vector>
#include <algorithm>
#include "../../Assertions.h"

namespace K {

	/** median filter for scalar values */
	template <typename Scalar> class Median {

		private:

			/** all scalar values in a sorted order (ascending) */
			std::vector<Scalar> sorted;

		public:

			/** add the given scalar value to the median calculation */
			void add(const Scalar value) {
				const auto idx = std::upper_bound( sorted.begin(), sorted.end(), value );
				sorted.insert( idx, value );
			}

			/** get the median of all added values */
			float get() const {

				// sanity check
				_assertNot0(sorted.size(), "add elements first!");

				// even or odd number of entries?
				if (sorted.size() % 2 == 1) {	// odd
					const int idx = (int) sorted.size()/2;
					return sorted[idx];
				} else {						// even
					const int idx0 = (int) sorted.size()/2 - 1;
					return (sorted[idx0] + sorted[idx0+1]) / 2;
				}

			}

	};

}


#endif // K_MATH_MEDIAN_H
