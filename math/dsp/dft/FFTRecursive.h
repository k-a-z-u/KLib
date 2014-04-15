#ifndef K_MATH_DSP_FFT_RECURSIVE_H
#define K_MATH_DSP_FFT_RECURSIVE_H

#include <complex>

namespace K {

	/**
	 * @brief this class uses a recursive implementation of the FFT
	 * which is quite straightforward but should be a little slower than
	 * the non-recursive version.
	 */
	template <int size> class FFTRecusive {

	public:

		/**
		 * @brief getComplexFFT
		 * do not forget to delete[] the result!
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template <typename type> std::complex<type>* getComplexFFT(type* values) {

			// allocate both buffers
			std::complex<type>* buf1 = new std::complex<type>[size];
			std::complex<type>* buf2 = new std::complex<type>[size];

			// set input for both buffers
			for (unsigned int i = 0; i < size; ++i) {
				buf1[i].real(values[i]);
				buf2[i].real(values[i]);
			}

			// perform recursive FFT
			_fft(buf1, buf2, size, 1);

			// cleanup
			delete[] buf1;

			// return result in buf2
			return buf2;

		}

		/**
		 * @brief perform iFFT on the given complext data in the frequncy domain
		 * this data will be modified inplace!
		 * do not forget to delete[] the returned array!
		 */
		template <typename type> type* getIFFT( std::complex<type>* in ) {

			// create second buffer
			std::complex<type>* out = new std::complex<type>[size];

			// change sign of imaginary part
			for (unsigned int i = 0; i < size; ++i) {
				in[i].imag( -in[i].imag() );
				out[i] = in[i];
			}

			// perform forward FFT
			_fft(in, out, size, 1);

			// adjust the given input values depending on the FFT size
			for (unsigned int i = 0; i < size; ++i) {
				out[i].real(  out[i].real() / size * 2 );
				out[i].imag( -out[i].imag() / size * 2 );
			}

			// create convenience output array
			type* res = new type[size];
			for (unsigned int i = 0; i < size; ++i) {
				res[i] = out[i].real();
			}

			// cleanup
			delete[] out;

			// done
			return res;

		}


	private:

		/**
		 * perform the actual FFT using both buffers.
		 * the result will then reside in the second buffer (out)
		 */
		template <typename type> void _fft(std::complex<type> buf[], std::complex<type> out[], int n, int step) {

			// lowest layer reached
			if (step >= n) {return;}

			_fft(out, buf, n, step * 2);
			_fft(out + step, buf + step, n, step * 2);

			for (unsigned int i = 0; i < n; i += 2 * step) {
				std::complex<type> t = std::polar(1.0, - PI * i / n) * out[i + step];
				buf[i / 2]     = out[i] + t;
				buf[(i + n)/2] = out[i] - t;
			}

		}

	};

}

#endif // K_MATH_DSP_FFT_RECURSIVE_H
