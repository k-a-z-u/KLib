#ifndef K_MATH_DSP_CONV_DSPCONVOLUTION_H
#define K_MATH_DSP_CONV_DSPCONVOLUTION_H

namespace K {

	/**
	 * @brief perform a normal (stupid) convolution with O(n^2)
	 */
	class DSPConvolution {

	public:


		/**
		 * @brief convolve the given two input signals a and b.
		 * the result will have a length of lenA+lenB.
		 * do not forget to delete[] the result!
		 */
		template <typename type> type* convolve(type* a, unsigned int lenA, type* b, unsigned int lenB) {

			// create output buffer with all zeros
			type* out = new type[lenA + lenB];
			memset(out, 0, sizeof(type) * (lenA+lenB));

			for (unsigned int i = 0; i < lenA; ++i) {
				for (unsigned int j = 0; j < lenB; ++j) {
					out[i+j] += a[i] * b[j];
				}
			}

			return out;

		}

	};

}

#endif // K_MATH_DSP_CONV_CONVOLUTION_H
