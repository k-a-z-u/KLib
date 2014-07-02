#ifndef K_MATH_DSP_WINDOW_H_
#define K_MATH_DSP_WINDOW_H_

namespace K {

	/**
	 * the base class for all DSP windowing functions
	 */
	template <typename type, int size> class DSPWindow {

	public:

		/** apply the window to the given data array */
		virtual void apply(type* data) = 0;

	protected:

		/** subclasses precompute the window function here once */
		float window[size];

	};

}

#endif //K_MATH_DSP_WINDOW_H_
