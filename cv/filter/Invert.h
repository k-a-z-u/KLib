#ifndef K_CV_FILTER_INVERT_H
#define K_CV_FILTER_INVERT_H

#include "../ImageChannel.h"

namespace K {

	namespace CV {

		/**
		 * invert an image.
		 */
		class Invert {

		public:

			/** invert the image */
			static void inplace(ImageChannel& img) {
				auto update = [&] (const int x, const int y, const float val) {(void) x; (void) y; return -val;};
				img.forEachModify(update);
			}

			static ImageChannel run(const ImageChannel& img) {
				ImageChannel out = img;
				inplace(out);
				return out;
			}

		};

	}

}

#endif // K_CV_FILTER_INVERT_H
