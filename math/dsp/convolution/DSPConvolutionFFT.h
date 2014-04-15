#ifndef K_MATH_DSP_CONV_DSPCONVOLUTIONFFT_H
#define K_MATH_DSP_CONV_DSPCONVOLUTIONFFT_H

#include "../dft/FFT.h"

namespace K {

	/**
	 * @brief perform signal convolution using the FFT
	 */
	template <typename type> class DSPConvolutionFFT {

	public:

		/** ctor */
		DSPConvolutionFFT() : curSize(0), fft(1024) {
			;
		}

		/** dtor */
		~DSPConvolutionFFT() {
			;
		}

		/** set the impulse response to convolve the input with */
		void setImpulseResponse(type* data, unsigned int len) {

			// create a copy
			impResp.resize(len);
			memcpy(impResp.data(), data, sizeof(type)*len);

		}


		/**
		 * @brief convolve the given two input signals a and b.
		 * the result will have a length of lenA+lenB.
		 * do not forget to delete[] the result!
		 */
		type* convolve(type* values, unsigned int len) {

			// the size of the convovled output
			const unsigned int newSize = len + (unsigned int) impResp.size();
			unsigned int fftLen = (unsigned int) std::pow(2,  std::ceil(std::log2(newSize))   );

			// update the impulse-response and internal buffers to match the new size
			reshape(fftLen);

			// create output buffer with all zeros
			paddedInput.resize(fftLen);
			memset(paddedInput.data(), 0, sizeof(type)*fftLen);
			memcpy(paddedInput.data(), values, sizeof(type)*len);

			// create FFT of the input
			CPLX* inputFFT = fft.getComplexFFT( paddedInput.data() );

			// multiply with impulse respons's FFT
			for (unsigned int i = 0; i < fftLen; ++i) {
				inputFFT[i] *= impRespFFT[i];
			}

			// perform iFFT
			type* out = fft.getIFFT(inputFFT);

			// cleanup
			delete inputFFT;

			// done
			return out;

		}


	private:

		/** adjust everything to ensure an FFT for the given size */
		void reshape(const unsigned int size) {

			// reshape needed?
			if (curSize == size) {return;}
			curSize = size;

			// adjust the FFT
			fft.resize(size);

			// calculate FFT of the impulse response
			// pad the impulse response with zeros and calculate FFT
			std::vector<type> tmp;
			tmp.resize(size);
			memcpy(tmp.data(), impResp.data(), sizeof(type)*impResp.size());
			impRespFFT = fft.getComplexFFT(tmp);

		}

	private:

		/** the impulse response to convolve with */
		std::vector<type> impResp;

		/** FFT version of the impulse response to convolve with */
		std::vector<CPLX> impRespFFT;

		/** the input padded with zeros at the end to match the FFT's size */
		std::vector<type> paddedInput;

		/** the currently used size for the FFT. might change depending on input */
		unsigned int curSize;

		/** the fft to use */
		FFT fft;

	};

}

#endif // K_MATH_DSP_CONV_DSPCONVOLUTIONFFT_H
