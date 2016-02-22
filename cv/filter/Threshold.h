#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "../ImageChannel.h"


namespace K {

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

	};

}

#endif // THRESHOLD_H
