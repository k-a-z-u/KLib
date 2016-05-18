#ifndef MATCHINGCONVOLUTION_H
#define MATCHINGCONVOLUTION_H

#include "../../geo/Point2.h"
#include "../ImageChannel.h"

namespace K {

	/**
	 * determine matching of two areas within two images
	 * using a window around a given point, examining
	 * the convolution of both windows
	 */
	class MatchingConvolution {

	private:

		const ImageChannel& img1;
		const ImageChannel& img2;
		int size;

	public:

		/** ctor with both images */
		MatchingConvolution(const ImageChannel& img1, const ImageChannel& img2, const int winSize = 7) : img1(img1), img2(img2), size(winSize/2) {
			;
		}

		/** get the matching-score between the two given regions */
		float getScore(const Point2i pImg1, const Point2i pImg2) const {

			float score = 0;

			for (int y = -size; y <= +size; ++y) {
				for (int x = -size; x <= +size; ++x) {

					const float v1 = img1.getClamped(pImg1.x+x, pImg1.y+y);
					const float v2 = img2.getClamped(pImg2.x+x, pImg2.y+y);
					score += (v1*v2);

				}
			}

			return score;

		}

	};

}

#endif // MATCHINGCONVOLUTION_H
