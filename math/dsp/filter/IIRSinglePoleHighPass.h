#ifndef K_MATH_DSP_FILTER_IIRSINGLEPOLEHIGHPASS_H
#define K_MATH_DSP_FILTER_IIRSINGLEPOLEHIGHPASS_H

#include "../../constants.h"

namespace K {

	/**
	 * @brief this class provides a single-pole IIR filter
	 * for a fast but very simple low-pass filtering.
	 */
	template <typename type> class IIRSinglePoleLowPass {

	public:

		/** ctor. "disabled" filter */
		IIRSinglePoleLowPass() : a0(1.0), b1(0.0) {
			;
		}

		/** get filtered version of the given ("continuous") input */
		type filter( const type in ) {

			// filter
			type ret = (in * a0) + (out1 * b1);
			out1 = ret;
			return ret;

		}

		/** set the cutoff-frequency (e.g. 1khz) depending on the sample-rate (e.g. 44khz) */
		void setCutoffFrequency( const double freq, const double sampleRate ) {

			// cutoff must be lower (or equal) than 0.5 * sample-rate
			assert(freq <= sampleRate / 2.0f);

			double fc = freq / sampleRate;
			double x = std::pow( K::E, (-2.0 * K::PI * fc) );

			a0 = 1 - x;
			b1 = x;

		}

		/** coefficient for the output value */
		type b1;

		/** coefficient for current input value */
		type a0;

		/** the last output value */
		type out1;

	};

}

#endif // K_MATH_DSP_FILTER_IIRSINGLEPOLEHIGHPASS_H
