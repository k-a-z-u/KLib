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
		static Kernel gauss1D(const float sigma, const int size = 3) {

			// (non-normalized) 1D gauss function
			// (we normalize the generated kernel instead of this function as the kernel's size is finite)
			auto func = [] (const int x, const float sigma) {
				return std::exp( - (float)((x*x)) / (2.0f*sigma*sigma) );
			};

			const int w = size;	int dx = w/2;
			Kernel k(w, 1);

			// calculate
			for (int x = 0; x < w; ++x) {
				k.set(x, 0, func(x-dx,sigma));
			}

			// ensure sum is 1.0
			k.normalize();
			return k;

		}

		/** get a 2D gauss kernel with the given sigma (same for x and y) */
		static Kernel gauss2D(const float sigma, const int size = 3) {

			// (non-normalized) 2D gauss function
			// (we normalize the generated kernel instead of this function as the kernel's size is finite)
			auto func = [] (const int x, const int y, const float sigma) {
				return std::exp( - (float)((x*x)+(y*y)) / (2.0f*sigma*sigma) );
			};

			const int w = size;	int dx = w/2;
			const int h = size;	int dy = h/2;
			Kernel k(w,h);

			// calculate
			for (int x = 0; x < w; ++x) {
				for (int y = 0; y < h; ++y) {
					k.set(x, y, func(x-dx,y-dy,sigma));
				}
			}

			// ensure sum is 1.0
			k.normalize();
			return k;

		}

	};

}

#endif // K_CV_KERNELFACTORY_H

