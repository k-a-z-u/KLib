#ifndef K_CV_RESIZE_H
#define K_CV_RESIZE_H

#include "../ImageChannel.h"

namespace K {

	namespace CV {

		class Resize {

		public:

			/** resize to the new width */
			template <typename Interpolator> static K::ImageChannel apply(const ImageChannel& img, const int w, const int h) {

				const float rw = (float)img.getWidth()	/ (float)w;
				const float rh = (float)img.getHeight()	/ (float)h;

				// the output image
				K::ImageChannel out(w, h);

				// calculate each output pixel
				for (int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {

						// get the pixel by interpolation
						const float px = Interpolator::get(img, x*rw, y*rh);
						out.set(x,y,px);

					}
				}

				// done
				return out;

			}


			template <typename Interpolator> static K::ImageChannel apply(const ImageChannel& img, const float scaler) {
				return apply<Interpolator>(img, (int)(img.getWidth() * scaler), (int)(img.getHeight() * scaler));
			}

		};

	}


}

#endif // K_CV_RESIZE_H
