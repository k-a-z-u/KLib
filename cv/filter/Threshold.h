#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "../ImageChannel.h"
#include "../Bitmap.h"

namespace K {

	namespace CV {

		/**
		 * convert an image to black/white
		 * using a threshold
		 */
		class Threshold {

		public:

			/** inplace convert the image to black/white */
			static void inplace(ImageChannel& img, const float threshold = 0.5f) {

				auto trans = [&] (const int x, const int y, const float v) {(void)x; (void)y; return (v > threshold) ? (1.0) : (0.0);};
				img.forEachModify(trans);

			}

			/** inplace convert the image to black/white */
			static ImageChannel get(ImageChannel& img, const float threshold = 0.5f) {

				ImageChannel out(img.getWidth(), img.getHeight());
				auto trans = [&] (const int x, const int y, const float v) { img.set( x, y, (v > threshold) ? (1.0) : (0.0) ) ;};
				img.forEach(trans);
				return out;

			}

			/** convert the given image into a true/false bitmap */
			static Bitmap bitmap(const ImageChannel& img, const float threshold = 0.5f, const bool invert = false) {

				// create new bitmap
				Bitmap out(img.getWidth(), img.getHeight());

				// fill
				auto trans = [&] (const int x, const int y, const float v) {
					if (!invert) {
						if (v > threshold) {out.set(x,y);} else {out.clear(x,y);}
					} else {
						if (v > threshold) {out.clear(x,y);} else {out.set(x,y);}
					}
				};
				img.forEach(trans);

				// done
				return out;

			}


		};

	}

}

#endif // THRESHOLD_H
