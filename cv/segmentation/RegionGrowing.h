#ifndef REGIONGROWING_H
#define REGIONGROWING_H

#include "../Point2.h"
#include "../ImageChannel.h"
#include "Segment.h"

#include <vector>
#include <unordered_set>

namespace K {

	/**
	 * image segmentation based on region growing
	 */
	class RegionGrowing {

	private:

		/** quickly track used pixels */
		struct Used : DataMatrix<bool> {
			Used(const int w, const int h) : DataMatrix(w, h) {;}
			void setUsed(const Point2i p) {setUsed(p.x, p.y);}
			void setUsed(const int x, const int y) {set(x,y,true);}
			bool isUsed(const Point2i p) {return isUsed(p.x, p.y);}
			bool isUsed(const int x, const int y) const {return get(x,y);}
		};

	public:

		/**
		 * get all other pixels beloging attached to the same seed.
		 * pixels count as attached when their difference is below the given threshold.
		 * @param seed the position to start searching
		 * @param threshold the difference threshold to use
		 * @return
		 */
		static Segment get(const ImageChannel& img, const Point2i& seed, const float threshold = 0.1f) {

			// track all visited points
			Used visited(img.getWidth(), img.getHeight());

			// track all to-be-checked points
			std::vector<Point2i> toCheck;

			// the detected region
			Segment seg;
			seg.avg = 0;

			// start at the seed
			toCheck.push_back(seed);
			visited.setUsed(seed);

			// while we have something to check
			while (!toCheck.empty()) {

				// get the next element to check the neighborhood for
				const Point2i next = toCheck.back();
				toCheck.erase(toCheck.end() - 1);
				const float val = img.get(next.x, next.y);

				// adjust the region (add point, update average value)
				seg.points.push_back(next);
				seg.avg += val;

				// check neighborhood
				for (int x = -1; x <= +1; ++x) {
					for (int y = -1; y <= +1; ++y) {

						// ignore the center
						if (x == y) {continue;}

						// get the new point
						Point2i p(next.x + x, next.y + y);

						// skip-out-of-bounds
						if ((p.x < 0) || (p.y < 0))				{continue;}
						if (p.x >= img.getWidth())				{continue;}
						if (p.y >= img.getHeight())				{continue;}

						// skip already visisted
						//if (visited.find(p) != visited.end())	{continue;}eed.x, seed.y
						if (visited.isUsed(p))					{continue;}

						// skip above-threshold
						const float diff = val - img.get(p.x, p.y);
						if (std::abs(diff) > threshold)			{continue;}

						// fine! -> add
						toCheck.push_back(p);
						visited.setUsed(p);

					}
				}


			}

			// adjust the average
			seg.avg /= (float) seg.points.size();

			// done
			return seg;

		}

	};

}

#endif // REGIONGROWING_H
