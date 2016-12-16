#ifndef K_CV_FILTER_NORMALIZE_H
#define K_CV_FILTER_NORMALIZE_H

#include "../ImageChannel.h"

namespace K {

	namespace CV {

		/**
		 * normalize an image:
		 * stretch its value range to ensure it ranges from 0.0 to 1.0
		 */
		class Normalize {

		public:

			/** normalize the image = set all values between [0.0:1.0]. min and max are autoamtically determined */
			static void inplace(ImageChannel& img) {

				const float min = *std::min_element(img.begin(), img.end());
				const float max = *std::max_element(img.begin(), img.end());
				const float diff = max - min;

				auto update = [&] (const int x, const int y, const float val) {(void) x; (void) y; return clamp01((val - min) / diff);};
				img.forEachModify(update);

			}

			/** normalize the image = set all values between [0.0:1.0] */
			static void inplace(ImageChannel& img, const float min, const float max) {

				const float diff = max - min;

				auto update = [&] (const int x, const int y, const float val) {(void) x; (void) y; return clamp01((val - min) / diff);};
				img.forEachModify(update);

			}

			/** normalize the image = set all values between [0.0:1.0] */
			static ImageChannel run(const ImageChannel& img) {
				ImageChannel out = img;
				inplace(out);
				return out;
			}

			/** normalize the image = set all values between [0.0:1.0] */
			static ImageChannel run(const ImageChannel& img, const float min, const float max) {
				ImageChannel out = img;
				inplace(out, min, max);
				return out;
			}

			static float clamp01(const float val) {
				if (val < 0) {return 0;}
				if (val > 1) {return 1;}
				return val;
			}


		};

	}

}

#endif // K_CV_FILTER_NORMALIZE_H
