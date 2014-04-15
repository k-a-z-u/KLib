#ifndef K_MATH_DSP_WINDOW_HAMMING_H_
#define K_MATH_DSP_WINDOW_HAMMING_H_

#include "DSPWindow.h"
#include "../../constants.h"

namespace K {

	/**
	 * create a hamming window of the given size which may then be applied
	 * to many data arrays.
	 *
	 * be aware that the size of the given array is NOT checked.
	 *
	 */
	template <typename T, int size> class DSPWindowHamming : DSPWindow<T, size> {

	public:

		/** ctor. setup the window array */
		DSPWindowHamming() {
			for (unsigned int i = 0; i < size; ++i) {
				this->window[i] = 0.54 - 0.46 * cos(2 * K::PI * i / size);
			}
		}

		void apply(T* data) override {
			for (unsigned int i = 0; i < size; ++i) {
				data[i] = data[i] * this->window[i];
			}
		}


	};

}

#endif // K_MATH_DSP_WINDOW_HAMMING_H_
