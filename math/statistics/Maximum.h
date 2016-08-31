#ifndef K_MATH_MAXIMUM_H
#define K_MATH_MAXIMUM_H

namespace K {

	template <typename Scalar> class Maximum {

	private:

		Scalar curMax;

	public:

		/** ctor */
		Maximum() : curMax(-999999999) {
			;
		}

		/** add a new value */
		void add(const Scalar val) {
			if (val > curMax) {curMax = val;}
		}

		/** get the current value */
		Scalar get() const {
			return curMax;
		}

	};

}

#endif // K_MATH_MAXIMUM_H
