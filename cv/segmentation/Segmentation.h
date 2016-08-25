#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Segment.h"
#include "../ImageChannel.h"
#include "RegionGrowing.h"

#include "unordered_set"

namespace K {

	class Segmentation {

	public:



		/**
		 * get all connected segments within the given image
		 * @param img the image to segmentize
		 * @param threshold the maximum difference to allow between two adjacent pixels
		 * @return
		 */

		static std::vector<Segment<float>> getSegments(const ImageChannel& img, const float threshold = 0.1f) {

			// track whether a pixel already belongs to a segment
			Bitmap used(img.getWidth(), img.getHeight());

			// run
			return getSegments(img, used, threshold);

		}

		/**
		 * get all connected segments within the given image
		 * @param img the image to segmentize
		 * @param used respect the given bitmap of already used (segmentized) pixels
		 * @param threshold the maximum difference to allow between two adjacent pixels
		 * @return
		 */
		static std::vector<Segment<float>> getSegments(const ImageChannel& img, Bitmap& used, const float threshold = 0.1f) {

			// all detected segments
			std::vector<Segment<float>> segments;

			// process all points of the image
			Point2i p(0,0);
			for (p.x = 0; p.x < img.getWidth(); ++p.x) {
				for (p.y = 0; p.y < img.getHeight(); ++p.y) {

					// skip all points already belonging to a segment
					if (used.isSet(p.x, p.y)) {continue;}

					// get the segment belonging to the seed "p"
					const Segment<float> s = RegionGrowing::get(img, p, used, threshold);
					segments.push_back(s);

				}
			}

			// done
			return segments;

		}



	};

}

#endif // SEGMENTATION_H
