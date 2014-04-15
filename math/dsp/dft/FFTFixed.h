#ifndef K_MATH_DSP_FFTFIXED_H
#define K_MATH_DSP_FFTFIXED_H

#include "../../constants.h"
#include <complex>
#include <cmath>

namespace K {

	/**
	 * @brief perform FFT using a non-recursive algorithm that also supports
	 * inplace modifictation of the provided data.
	 *
	 * the fixed version might be faster than the dynamic version but requires
	 * the size to be known at runtime
	 *
	 */
	template<int size> class FFTFixed {

	public:

		/** ctor */
		FFTFixed() {

			this->bits = (unsigned int) std::ceil( std::log2(size) );

			// build bit-reverse LUT
			assert(size <= 65536); // LUT maximum
			for (unsigned int i = 0; i < size; ++i) {
				bitReverse[i] = (uint16_t) _reverseBits(i, bits);
			}

		}

		/**
		 * @brief get a complex result for the given real values (imag = 0.0)
		 * this will OVERWRITE the given input by working on it inplace.
		 *
		 * @param values the amplitude vector to get the FFT for. image = 0.0
		 */
		template <typename type> void getComplexFFT( std::complex<type>* values ) {

			// re-order entries by swapping bits (left for right)
			for (unsigned int i = 0; i < size; ++i) {
				unsigned int j = bitReverse[i];
				if (i < j) {
					std::complex<type> tmp = values[i];
					values[i] = values[j];
					values[j] = tmp;
				}
			}

			// perform FFT
			fftOnSwappedArray( values );

		}


		/**
		 * @brief get a complex result for the given real values.
		 * do not forget to delete[] the result!
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template <typename type> std::complex<type>* getComplexFFT( const type* values ) {

			// allocate output buffer
			std::complex<type>* out = new std::complex<type>[size];

//			// create a complex version of the real array
//			// and thereby re-order it by swapping the bits (left for right)
//			for (unsigned int i = 0; i < size; ++i) {
//				unsigned int idx = bitReverse[i];
//				out[idx].real(values[i]);
//			}

//			// perform FFT
//			fftOnSwappedArray(out);

			// fill the allocated complex array with the real values
			for (unsigned int i = 0; i < size; ++i) {
				out[i].real( values[i] );
			}

			// perform inplace, complex FFT
			getComplexFFT( out );

			// done
			return out;

		}

		/**
		 * @brief get real-only result for the given real values.
		 * do not forget to delete[] the result!
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template<typename type> type* getRealFFT( const type* values ) {

			// calculate complex FFT
			// TODO any faster solutions here?? directly operate on real-only values?
			std::complex<type>* complex = getComplexFFT(values);

			// output result
			unsigned int len = size/2+1;
			type* real = new type[len];

			// calculate real values
			for (unsigned int i = 0; i < len; ++i) {
				real[i] = std::sqrt( std::abs( (complex[i] * complex[i]).real() ) );
			}

			// delete temporal buffer
			delete[] complex;

			return real;

		}

		/**
		 * @brief perform iFFT on the given complext data in the frequncy domain
		 * this data will be modified inplace!
		 * do not forget to delete[] the returned array!
		 */
		template <typename type> type* getIFFT( std::complex<type>* in ) {

			// change sign of imaginary part
			for (unsigned int i = 0; i < size; ++i) {
				in[i].imag( -in[i].imag() );
			}

			// perform forward FFT
			getComplexFFT(in);

			// adjust the given input values depending on the FFT size
			for (unsigned int i = 0; i < size; ++i) {
				in[i].real(  in[i].real() / size );
				in[i].imag( -in[i].imag() / size );
			}

			// create convenience output array
			type* out = new type[size];
			for (unsigned int i = 0; i < size; ++i) {
				out[i] = in[i].real();
			}
			return out;

		}



	private:


		/**
		 * perform a non-recursive,complex,inplace FFT on the given complex array
		 * which must already have been bit-swapped.
		 */
		template <typename type> __attribute__((optimize("unroll-loops")))  void fftOnSwappedArray( std::complex<type>* x ) const {

			for (unsigned int L = 2; L <= size; L += L) {
				for (unsigned int k = 0; k < L/2; ++k) {

					type kth = (type) ( (type) -2.0 * (type) k * (type) K::PI / (type) L);

					std::complex<type> w(std::cos(kth), std::sin(kth));
					//std::complex<type> w = std::polar(1.0, kth);

					for (unsigned int j = 0; j < size/L; ++j) {

						std::complex<type> tao = w * x[j*L + k + L/2];

						x[j*L + k + L/2] = x[j*L + k] - (tao);
						x[j*L + k]       = x[j*L + k] + (tao);
					}
				}
			}

		}

		/** reverse the bits of the given 32 bit int */
		unsigned int _reverse(register uint32_t x) const {
			x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
			x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
			x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
			x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
			return((x >> 16) | (x << 16));
		}

		/** reverse only the least significant bits of the given int */
		unsigned int _reverseBits(uint32_t x, unsigned int bits) const {
			return _reverse(x) >> ( sizeof(x)*8 - bits );
		}



	private:

		/** the number of bits, depending on the length */
		unsigned int bits;

		/** LUT for bit-swapping (left for right) */
		uint16_t bitReverse[size];

	};



}

#endif // K_MATH_DSP_FFTFIXED_H
