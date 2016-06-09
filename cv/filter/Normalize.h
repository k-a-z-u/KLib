#ifndef NORMALIZE_H
#define NORMALIZE_H

#include "../ImageChannel.h"

namespace K {

	/**
	 * normalize an image:
	 * stretch its value range to ensure it ranges from 0.0 to 1.0
	 */
	class Normalize {

	public:

		/** normalize the image = set all values between [0.0:1.0] */
		static void inplace(ImageChannel& img) {

			const float min = *std::min_element(img.begin(), img.end());
			const float max = *std::max_element(img.begin(), img.end());
			const float diff = max - min;

			auto update = [&] (const int x, const int y, const float val) {(void) x; (void) y; return (val - min) / diff;};
			img.forEachModify(update);

		}

		/** normalize the image = set all values between [0.0:1.0] */
		static ImageChannel run(ImageChannel& img) {

			// copy
			ImageChannel out = img;

			// normalize
			inplace(out);

			return out;

		}

	};


}

#endif // NORMALIZE_H
