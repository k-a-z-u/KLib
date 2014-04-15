#ifndef K_MATH_DSP_WINDOW_BARTLETT_H_
#define K_MATH_DSP_WINDOW_BARTLETT_H_

#include "DSPWindow.h"
#include "../../constants.h"

namespace K {

	/**
	 * create a bartlett window of the given size which may then be applied
	 * to many data arrays.
	 *
	 * be aware that the size of the given array is NOT checked.
	 *
	 */
	template <typename T, int size> class DSPWindowBartlett : DSPWindow<T, size> {

	public:

		/** ctor. setup the window array */
		DSPWindowBartlett() {
			for (unsigned int i = 0; i < size; ++i) {
				this->window[i] = (2.0/(size-1)) * ( ((size-1)/2.0) - std::abs( i-((size-1)/2.0) ) );
			}
		}

		void apply(T* data) override {
			for (unsigned int i = 0; i < size; ++i) {
				data[i] = data[i] * this->window[i];
			}
		}


	};

}

#endif // K_MATH_DSP_WINDOW_BARTLETT_H_
