#ifndef K_MATH_AVERAGE_H
#define K_MATH_AVERAGE_H

#include "../../Assertions.h"

namespace K {

	template <typename Scalar> class Average {

	private:

		int cnt;
		Scalar sum;

	public:

		/** ctor */
		Average() : cnt(0), sum(0) {
			;
		}

		/** get the number of elements */
		int size() const {
			return cnt;
		}

		/** add a new value */
		void add(const Scalar val) {
			sum += val;
			++cnt;
		}

		/** get the current value */
		Scalar get() const {
			_assertNot0(cnt, "add() values first!");
			return sum / (Scalar)cnt;
		}

	};

}

#endif // K_MATH_AVERAGE_H
