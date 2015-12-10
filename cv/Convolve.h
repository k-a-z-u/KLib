#ifndef K_CV_CONVOLVE_H
#define K_CV_CONVOLVE_H

#include "ImageChannel.h"
#include "Kernel.h"

#include <cmath>

namespace K {

	class Convolve {

	public:

		/**
		 * convolve the given image with the provided kernel.
		 * returns a newly created image.
		 */
		static ImageChannel run(const ImageChannel& src, const Kernel& k) {

			ImageChannel dst(src.getWidth(), src.getHeight());

			#pragma omp parallel for
			for (int y = 0; y < src.getHeight(); ++y) {
				for (int x = 0; x < src.getWidth(); ++x) {

					const int dx = k.getWidth() / 2;
					const int dy = k.getHeight() / 2;

					float val = 0;		// the value after convolving all pixels
					float sum = 0;		// used for normalization (unnormalized kernels / edges [less values ues])

					// convolve the current pixel with the kernel
					for (int y1 = 0; y1 < k.getHeight(); ++y1) {
						for (int x1 = 0; x1 < k.getWidth(); ++x1) {

							const int ix = x+x1-dx;
							const int iy = y+y1-dy;

							// skip edges ?
							if (ix < 0 || ix >= src.getWidth())		{continue;}
							if (iy < 0 || iy >= src.getHeight())	{continue;}

							const float kv = k.get(x1, y1);		// kernel's value
							const float sv = src.get(ix, iy);	// source image's value
							val += kv*sv;
							sum += kv;

						}
					}

					// set the normalized, convolved value
					dst.set(x, y, val/sum);

				}
			}

			return dst;

		}




	};

}

#endif // K_CV_CONVOLVE_H

