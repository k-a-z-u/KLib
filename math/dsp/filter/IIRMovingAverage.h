#ifndef K_MATH_DSP_IIRMOVINGAVERAGE_H
#define K_MATH_DSP_IIRMOVINGAVERAGE_H

namespace K {

	/**
	 * @brief provides an IIR-like filter using moving average
	 * of a given size
	 */
	template <typename type, int size> class IIRMovingAverage {

	public:

		/** ctor */
		IIRMovingAverage() : movingSum(0), values(), oldest(0) {
			;
		}

		/** filter the given input value */
		type filter(type in) {

			movingSum += in;
			movingSum -= values[oldest];

			values[oldest] = in;
			++oldest;
			oldest %= size;

			return movingSum / size;

		}

	private:

		/** current sum of all inserted values */
		type movingSum;

		/** store old values to adjust the moving sum */
		type values[size];

		/** index to access the oldes value */
		int oldest;

	};

}

#endif // K_MATH_DSP_IIRMOVINGAVERAGE_H
