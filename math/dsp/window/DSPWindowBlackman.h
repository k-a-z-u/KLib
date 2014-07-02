#ifndef K_MATH_DSP_WINDOW_BLACKMAN_H_
#define K_MATH_DSP_WINDOW_BLACKMAN_H_

#include "DSPWindow.h"
#include "../../constants.h"

namespace K {

	/**
	 * create a blackman window of the given size which may then be applied
	 * to many data arrays.
	 *
	 * be aware that the size of the given array is NOT checked.
	 *
	 */
	template <typename type, int size> class DSPWindowBlackman : DSPWindow<type, size> {

	public:

		/** ctor. setup the window array */
		DSPWindowBlackman() {
			for (unsigned int i = 0; i < size; ++i) {
				this->window[i] = ( 0.42 - 0.5 * cos(2 * K::PI * i / size) + 0.08 * cos(4 * K::PI * i / size) );
			}
		}

		void apply(type* data) override {
			for (unsigned int i = 0; i < size; ++i) {
				data[i] = data[i] * this->window[i];
			}
		}


	};

}

#endif //K_MATH_DSP_WINDOW_BLACKMAN_H_
