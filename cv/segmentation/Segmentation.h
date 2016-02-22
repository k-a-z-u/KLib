#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Segment.h"
#include "../ImageChannel.h"
#include "RegionGrowing.h"

#include "unordered_set"

namespace K {

	class Segmentation {

	private:

		/** quickly track used pixels */
		struct Used : DataMatrix<bool> {
			Used(const int w, const int h) : DataMatrix(w, h) {;}
			void setUsed(const int x, const int y) {set(x,y,true);}
			bool isUsed(const int x, const int y) const {return get(x,y);}
		};

	public:

		/**
		 * get all connected segments within the given image
		 * @param img
		 * @return
		 */
		static std::vector<Segment> getSegments(const ImageChannel& img, const float threshold = 0.1f) {

			// all detected segments
			std::vector<Segment> segments;

			// all used points
			Used used(img.getWidth(), img.getHeight());

			// process all points of the image
			Point2i p(0,0);
			for (p.x = 0; p.x < img.getWidth(); ++p.x) {
				for (p.y = 0; p.y < img.getHeight(); ++p.y) {

					// skip all points already belonging to a segment
					if (used.isUsed(p.x, p.y)) {continue;}

					// get the segment belonging to the seed "p"
					const Segment s = RegionGrowing::get(img, p, threshold);
					segments.push_back(s);

					// mark all segment-points as "used"
					for (const Point2i p : s.points) {used.setUsed(p.x, p.y);}

				}
			}

			// done
			return segments;

		}



	};

}

#endif // SEGMENTATION_H
