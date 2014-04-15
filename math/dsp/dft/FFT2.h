#ifndef K_MATH_DSP_FFT2_H
#define K_MATH_DSP_FFT2_H

#include "../../constants.h"
#include <complex>
#include <cmath>
#include <vector>
#include <cassert>
#include "ComplexBuffer.h"

namespace K {

	/**
	 * @brief perform FFT using a non-recursive algorithm that also supports
	 * inplace modifictation of the provided data.
	 *
	 * the FFT will be initialized for a specific size that must be a power of 2.
	 * necessary look-up-tables etc will thereby be created and kept internally.
	 *
	 */
	class FFT2 {

	public:

		/**
		 * @brief ctor for a FFT using the given size
		 * @param size the number of entries to use. must be a power of 2 (64, 128, 256, ...)
		 */
		FFT2(const unsigned int size) : size(0), bitReverse(nullptr) {
			resize(size);
		}

		/** dtor */
		~FFT2() {

			// cleanup
			delete[] bitReverse;
			bitReverse = 0;

		}

		/**
		 * @brief change the size to use for the fft.
		 * must be a power of 2.
		 * @param size the size to use.
		 */
		void resize(const unsigned int size) {

			// size changed?
			if (this->size == size) {return;}

			// size must be a power of 2
			assert( std::log2(size) == (int) std::log2(size) );

			// the bit-reverse LUT is currently limited to 64k
			assert(size <= 65536);

			// the new size and corresponding number of bits
			this->size = size;
			this->bits = (unsigned int) std::ceil( std::log2(size) );

			// remove previous data
			if (bitReverse) {
				delete[] bitReverse;
				bitReverse = 0;
			}

			// build bit-reverse LUT
			bitReverse = new uint16_t[size];
			for (unsigned int i = 0; i < size; ++i) {
				bitReverse[i] = (uint16_t) _reverseBits(i, bits);
			}

		}

		/** get the currently configured size */
		unsigned int getSize() const {
			return this->size;
		}


		/**
		 * @brief get a complex result for the given real values (imag = 0.0)
		 * this will OVERWRITE the given input by working on it inplace.
		 *
		 * @param values the amplitude vector to get the FFT for. image = 0.0
		 */
		template <typename type> void getComplexFFT( ComplexBuffer<type>& complex ) const {

			// re-order entries by swapping bits (left for right)
			for (unsigned int i = 0; i < size; ++i) {
				const unsigned int j = bitReverse[i];
				if (i < j) {
					complex.swap(i, j);
				}
			}

			// perform FFT
			fftOnSwappedArray( complex );

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
		template <typename type> void fftOnSwappedArray( ComplexBuffer<type>& x ) const {

			const unsigned int size = this->size;
			const type PI2 = -2.0 * K::PI;

			for (unsigned int L = 2; L <= size; L += L) {
				for (unsigned int k = 0; k < L/2; ++k) {

					// complex value to adjust each lower-level FFTs while "bubbling upwards"#
					const type kth = (type) ( PI2 * (type) k / (type) L);
					const type r2 = std::cos(kth);
					const type i2 = std::sin(kth);

					for (unsigned int j = 0; j < size/L; ++j) {

						const unsigned int idx1 = j*L + k + L/2;
						const unsigned int idx2 = j*L + k;

						const type r1 = x.realValues[idx1];
						const type i1 = x.imagValues[idx1];

						// complex multiplication of (r1, i1) * (r2, i2)
						type r = (r1*r2 - i1*i2);
						type i = (r1*i2 + i1*r2);

						// complex subtraction
						x.realValues[idx1] = x.realValues[idx2] - r;
						x.imagValues[idx1] = x.imagValues[idx2] - i;

						// complex addition
						x.realValues[idx2] = x.realValues[idx2] + r;
						x.imagValues[idx2] = x.imagValues[idx2] + i;

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

		/** the size of the FFT to use */
		unsigned int size;

		/** the number of bits, depending on the length */
		unsigned int bits;

		/** LUT for bit-swapping (left for right) */
		uint16_t* bitReverse;

	};



}

#endif // K_MATH_DSP_FFT_H
