#ifndef MATCHINGSAD_H
#define MATCHINGSAD_H

#include "../Point2.h"
#include "../ImageChannel.h"

namespace K {

	/**
	 * determine matching of two areas within two images
	 * using a window around a given point, examining
	 * the square of absolute differences between both windows
	 */
	class MatchingSAD {

	private:


		const ImageChannel& img1;
		const ImageChannel& img2;
		int size;

	public:

		/** ctor with both images */
		MatchingSAD(const ImageChannel& img1, const ImageChannel& img2, const int winSize = 7) : img1(img1), img2(img2), size(winSize/2) {
			;
		}

		/** get the error between the two given regions */
		float getError(const Point2i pImg1, const Point2i pImg2) const {

			float err = 0;

			for (int y = -size; y <= +size; ++y) {
				for (int x = -size; x <= +size; ++x) {

					const float v1 = img1.getClamped(pImg1.x+x, pImg1.y+y);
					const float v2 = img2.getClamped(pImg2.x+x, pImg2.y+y);
					err += std::abs(v1-v2);

				}
			}

			return err;

		}

	};

}

#endif // MATCHINGSAD_H
