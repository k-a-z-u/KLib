#ifndef K_CV_SOBEL_H
#define K_CV_SOBEL_H

#include "../ImageChannel.h"
#include "../Convolve.h"

namespace K {

	namespace CV {

		class Sobel {

		public:

			static void inplace(ImageChannel& img) {

				img = apply(img);

			}

			static K::ImageChannel apply(ImageChannel& img) {

//				K::ImageChannel resX = Convolve::run(img, getKernelX(), false);
//				K::ImageChannel resY = Convolve::run(img, getKernelY(), false);
//				K::ImageChannel res = resX; res += resY;
//				res += 0.5f;
//				return res;

				K::ImageChannel res = Convolve::run(img, getKernelXY(), true);
				//res /= 12.0f;
				res += 0.5f;	// center around 50%
				return res;

			}

			static Kernel getKernelXY() {
				float values[9] = {
					-2, -2,  0,
					-2,  0,  2,
					 0,  2,  2,
				};
				return Kernel(values, 3,3);
			}

			/** get a 1D-X sobel kernel */
			static Kernel getKernelX() {
				float values[9] = {
					-1, 0, +1,
					-2, 0, +2,
					-1, 0, +1
				};
				return Kernel(values, 3, 3);
			}

			/** get a 1D-Y sobel kernel */
			static Kernel getKernelY() {
				float values[9] = {
					+1, +2, +1,
					 0,  0,  0,
					-1, -2, -1
				};
				return Kernel(values, 3, 3);
			}



		};

	}


}

#endif // K_CV_SOBEL_H
