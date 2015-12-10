#ifndef K_CV_KERNELFACTORY_H
#define K_CV_KERNELFACTORY_H

#include "Kernel.h"
#include <cmath>

namespace K {

	/**
	 * see:
	 * http://mathworld.wolfram.com/GaussianFunction.html
	 */
	class KernelFactory {

	public:

		/** get a 1D gauss kernel */
		static Kernel gauss1D(const float sigma, int size = 0) {

			// default size? -> auto-detect
			if (size == 0) {size = getGaussSize(sigma);}

			// (non-normalized) 1D gauss function
			// (we normalize the generated kernel instead of this function as the kernel's size is finite)
			auto func = [] (const int x, const float sigma) {
				return std::exp( - (float)((x*x)) / (2.0f*sigma*sigma) );
			};

			const int w = size; const int dx = w/2;
			Kernel k(w, 1);

			// calculate
			for (int x = 0; x < w; ++x) {
				k.set(x, 0, func(x-dx, sigma));
			}

			// ensure sum is 1.0
			k.normalize();
			return k;

		}

		/** get a 2D gauss kernel with the given sigma (same for x and y) */
		static Kernel gauss2D(const float sigma, int size = 0) {

			// default size? -> auto-detect
			if (size == 0) {size = getGaussSize(sigma);}

			// (non-normalized) 2D gauss function
			// (we normalize the generated kernel instead of this function as the kernel's size is finite)
			auto func = [] (const int x, const int y, const float sigma) {
				return std::exp( - (float)((x*x)+(y*y)) / (2.0f*sigma*sigma) );
			};

			const int w = size;	const int dx = w/2;
			const int h = size;	const int dy = h/2;
			Kernel k(w,h);

			// calculate
			for (int x = 0; x < w; ++x) {
				for (int y = 0; y < h; ++y) {
					k.set(x, y, func(x-dx, y-dy, sigma));
				}
			}

			// ensure sum is 1.0
			k.normalize();
			return k;

		}

	private:

		/** convert the given gauss-sigma to a (required) kernel size */
		static int getGaussSize(const float sigma) {
			int size = (int) (sigma * 6.0f);		// size is ~ sigma * 6;
			if ((size % 2) == 0) {size += 1;}		// ensure the size is uneven
			if (size < 3) {size = 3;}				// ensure the size is at-least 3
			return size;
		}

	};

}

#endif // K_CV_KERNELFACTORY_H

