#ifndef K_CV_ROTATE_H
#define K_CV_ROTATE_H

#include "../ImageChannel.h"
#include "Interpolation.h"
#include "../../geo/Point2.h"

namespace K {

	namespace CV {

		class Rotate {

		public:

			/** resize to the new width */
			template <typename Interpolator> static ImageChannel apply(const ImageChannel& img, const Point2f center, const float rad) {

				// the output image
				const int w = img.getWidth();
				const int h = img.getHeight();
				K::ImageChannel out(w, h);

				// calculate each output pixel
				for (int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {

						// ensure center is (0,0)
						K::Point2f p1 = K::Point2f((float)x, (float)y) - center;

						// rotate around (0,0)
						p1.rotate(rad);

						// re-apply center
						K::Point2f pi = p1 + center;

						// get the pixel by interpolation and set it
						const float px = Interpolator::get(img, pi.x, pi.y);
						out.set(x,y,px);

					}
				}

				// done
				return out;

			}

		};

	}


}

#endif // K_CV_ROTATE_H
