#ifndef K_CV_MEDIAN_H
#define K_CV_MEDIAN_H

#include "../ImageChannel.h"
#include "../../Assertions.h"
#include "../../math/statistics/Median.h"

namespace K {

	namespace CV {

		/** median image filter */
		class Median {

		public:

			/** apply a median-filter to the given image */
			static ImageChannel apply(const ImageChannel& img, const int sx = 3, const int sy = 3) {

				K::ImageChannel res(img.getWidth(), img.getHeight());

				for (int y = 0; y < img.getHeight(); ++y) {
					for (int x = 0; x < img.getWidth(); ++x) {
						res.set(x,y, get(img,x,y,sx,sy));
					}
				}

				return res;

			}

			/** get the median for the given (x,y) by examining its neighborhood (default 3x3) */
			static float get(const ImageChannel& img, const int x, const int y, const int sx = 3, const int sy = 3) {

				// sanity checks
				_assertTrue(sx % 2 == 1, "sx must be odd");
				_assertTrue(sy % 2 == 1, "sx must be odd");

				// boundary (positive and negative)
				const int rx = (sx-1)/2;
				const int ry = (sy-1)/2;

				const int x1 = img.clampX(x-rx);
				const int x2 = img.clampX(x+rx);

				const int y1 = img.clampY(y-ry);
				const int y2 = img.clampY(y+ry);

				// add neighborhood
				K::Median<float> med;
				for (int yy = y1; yy <= y2; ++yy) {
					for (int xx = x1; xx <= x2; ++xx) {
						const float val = img.get(xx,yy);
						med.add(val);
					}
				}

				// done
				return med.get();

			}

		};

	}

}

#endif // K_CV_MEDIAN_H
