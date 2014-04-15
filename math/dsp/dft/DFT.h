#ifndef K_MATH_DSP_DFT_H_
#define K_MATH_DSP_DFT_H_

#include "../../constants.h"
//#include "../../vector/Matrix.h"
//#include "../../vector/Vector.h"

/**
 * this class calculates the DFT (and iDFT) the stupid way
 * by using simple convolution with the corresponding sine
 * and cosine waves.
 *
 * the algorithm is thus O(N^2) and should not be used for
 * sizes above 1024 samples.
 *
 * on the other hand, the algorithm is easy to understand
 * and works with any sample size and must not necessarily
 * use a power of 2.
 */

namespace K {

	template <typename type> struct DFTPolar;
	template <typename type> struct DFTRectangular;

	/** polar representation of the frequency domain */
	template <typename type> struct DFTPolar {

		type mag;
		type phase;

		DFTPolar() : mag(0), phase(0) {;}
		DFTPolar(type mag, type phase) : mag(mag), phase(phase) {;}

		DFTRectangular<type> getRectangular();

	};

	/** rectangular representation of the frequency domain */
	template <typename type> struct DFTRectangular {

		type sin;
		type cos;

		DFTRectangular() : sin(0), cos(0) {;}
		DFTRectangular(type sin, type cos) : sin(sin), cos(cos) {;}

		/** convert to polar notation */
		DFTPolar<type> getPolar();

	};

	template <typename type> DFTRectangular<type> DFTPolar<type>::getRectangular(){
		type sin = mag * cos(phase);
		type cos = mag * sin(phase);
		return DFTRectangular<type>(sin, mag);
	}

	template <typename type> DFTPolar<type> DFTRectangular<type>::getPolar() {
		type mag = std::sqrt(sin*sin + cos*cos);
		type phase = std::atan(sin/cos);
		return DFTPolar<type>(mag, phase);
	}



	template <typename type, unsigned int size> class DFT {

	public:

		/** ctor. precompute the convolution kernel */
		DFT() {

			// build matrices for sine and cosine wave
			for (unsigned int i = 0; i < size/2+1; ++i) {
				for (unsigned int x = 0; x < size; ++x) {
					convCos.set(x,i, cos(2 * K::PI * i * x / size));
					convSin.set(x,i, sin(2 * K::PI * i * x / size));
				}
			}


		}


		/** get DFT for the given input array into the given output */
		void getDFT(type* in, DFTRectangular<type>* out) {

			for (unsigned int i = 0; i < size/2+1; ++i) {
				out[i].sin = 0;
				out[i].cos = 0;
				for (unsigned int x = 0; x < size; ++x) {
					out[i].sin += in[x] * convSin.get(x,i);
					out[i].cos += in[x] * convCos.get(x,i);
				}
			}

		}

		/** get the iDFT for the given input into the given output array */
		void getIDFT(DFTRectangular<type>* in, type* out) {

			for (unsigned int x = 0; x < size; ++x) {
				out[x] = 0;
				for (unsigned int i = 0; i < size/2+1; ++i) {
					out[x] += in[i].sin * convSin.get(x,i);
					out[x] += in[i].cos * convCos.get(x,i);
				}
				out[x] /= size/2;
			}

		}



		/** simple helper for 2D array access */
		struct ConvMat {
			double data[ size * (size/2+1) ];
			double get(unsigned int x, unsigned int y) const {return data[x+y*size];}
			void set(unsigned int x, unsigned int y, double v) {data[x+y*size] = v;}
		};

		/** convolution matrix for sine wave */
		ConvMat convSin;

		/** convolution matrix for cosine wave */
		ConvMat convCos;

	};

}

#endif //K_MATH_DSP_DFT_H_
