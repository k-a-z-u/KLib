#ifndef K_MATH_DSP_FFT_H
#define K_MATH_DSP_FFT_H

#include "../../constants.h"
#include <complex>
#include <cmath>
#include <vector>
#include <cassert>
#include "Complex.h"

#define CPLX Complex<type>
//#define CPLX std::complex<type>

namespace K {

	/**
	 * @brief perform FFT using a non-recursive algorithm that also supports
	 * inplace modifictation of the provided data.
	 *
	 * the FFT will be initialized for a specific size that must be a power of 2.
	 * necessary look-up-tables etc will thereby be created and kept internally.
	 *
	 */
	class FFT {

	public:

		/**
		 * @brief ctor for a FFT using the given size
		 * @param size the number of entries to use. must be a power of 2 (64, 128, 256, ...)
		 */
		FFT(const unsigned int size) : size(0), bitReverse(nullptr) {
			resize(size);
		}

		/** dtor */
		~FFT() {

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
		template <typename type> void getComplexFFT( CPLX* values ) const {

			// re-order entries by swapping bits (left for right)
			for (unsigned int i = 0; i < size; ++i) {
				const unsigned int j = bitReverse[i];
				//const unsigned int j = _reverseBits(i, bits);
				if (i < j) {
					CPLX tmp = values[i];
					values[i] = values[j];
					values[j] = tmp;
				}
			}

			// perform FFT
			fftOnSwappedArray( values );

		}

		/**
		 * @brief get a complex result for the given real values (imag = 0.0)
		 * this will OVERWRITE the given, pre-allocated output buffer
		 *
		 * @param values the amplitude vector to get the FFT for. image = 0.0
		 * @param out the complex buffer to write the result to
		 */
		template <typename type> void getComplexFFT( type* values, CPLX* out ) const {

			// fill the complex array with the real values
			for (unsigned int i = 0; i < size; ++i) {
				out[i].real( values[i] );
				out[i].imag( 0 );
			}

			// perform inplace, complex FFT
			getComplexFFT( out );

		}

		/**
		 * @brief get a complex result for the given real values (imag = 0.0)
		 * this will OVERWRITE the given input by working on it inplace.
		 *
		 * @param values the amplitude vector to get the FFT for. image = 0.0
		 */
		template <typename type> void getComplexFFT( std::vector< CPLX > values ) const {

			// ensure correct vector size
			assert(values.size() == size);

			// run
			getComplexFFT( values.data() );

		}


		/**
		 * @brief get a complex result for the given real values.
		 * do not forget to delete[] the result!
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template <typename type> CPLX* getComplexFFT( const type* values ) const {

			// allocate output buffer
			CPLX* out = new CPLX[size];

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
		 * @brief get a complex result for the given real values.
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template <typename type> std::vector< CPLX > getComplexFFT( const std::vector<type> values ) const {

			// allocate output buffer
			std::vector<CPLX> out;
			out.resize(size);

			// fill the allocated complex array with the real values
			for (unsigned int i = 0; i < size; ++i) {
				out[i].real( values[i] );
			}

			// perform inplace, complex FFT
			getComplexFFT( out.data() );

			// done
			return out;

		}

		/**
		 * @brief get real-only result for the given real values.
		 * do not forget to delete[] the result!
		 *
		 * @param values the amplitude vector to get the FFT for
		 */
		template<typename type> type* getRealFFT( const type* values ) const {

			// alloc output result
			const unsigned int len = size/2+1;
			type* real = new type[len];

			// get real FFT
			getRealFFT(values, real);

			return real;

		}

		/**
		 * @brief get real-only result for the given real values.
		 * make sure "out" holds size/2+1 entries!
		 *
		 * @param values the amplitude vector to get the FFT for
		 * @param out the result of the real FFT. size/2+1 entries
		 */
		template<typename type> void getRealFFT( const type* values, type* out) const {

			// calculate complex FFT
			// TODO any faster solutions here?? directly operate on real-only values?
			CPLX* complex = getComplexFFT(values);

			// calculate real values
			const unsigned int len = size/2+1;
			for (unsigned int i = 0; i < len; ++i) {
				out[i] = std::sqrt( std::abs( (complex[i] * complex[i]).real() ) );
			}

			// delete temporal buffer
			delete[] complex;

		}



		/**
		 * @brief perform iFFT on the given complext data in the frequncy domain
		 * this data will be modified inplace!
		 * do not forget to delete[] the returned array!
		 */
		template <typename type> type* getIFFT( CPLX* in ) {

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
		template <typename type> void fftOnSwappedArray( CPLX* x ) const {

			for (unsigned int L = 2; L <= size; L += L) {
				for (unsigned int k = 0; k < L/2; ++k) {

					type kth = (type) ( (type) -2.0 * (type) k * (type) K::PI / (type) L);

					CPLX w(std::cos(kth), std::sin(kth));
					//CPLX w = std::polar(1.0, kth);

					for (unsigned int j = 0; j < size/L; ++j) {

						CPLX tao = w * x[j*L + k + L/2];

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

		/** the size of the FFT to use */
		unsigned int size;

		/** the number of bits, depending on the length */
		unsigned int bits;

		/** LUT for bit-swapping (left for right) */
		uint16_t* bitReverse;

	};



}

#endif // K_MATH_DSP_FFT_H
