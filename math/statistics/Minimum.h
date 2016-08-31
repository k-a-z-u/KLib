#ifndef K_MATH_MINIMUM_H
#define K_MATH_MINIMUM_H

namespace K {

	template <typename Scalar> class Minimum {

	private:

		Scalar curMin;

	public:

		/** ctor */
		Minimum() : curMin(+999999999) {
			;
		}

		/** add a new value */
		void add(const Scalar val) {
			if (val < curMin) {curMin = val;}
		}

		/** get the current value */
		Scalar get() const {
			return curMin;
		}

	};

}

#endif // K_MATH_MINIMUM_H
