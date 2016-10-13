#ifndef REGIONGROWING_H
#define REGIONGROWING_H

#include "../../geo/Point2.h"
#include "../ImageChannel.h"
#include "../Bitmap.h"

#include "Segment.h"

#include <vector>
#include <unordered_set>

namespace K {

	/**
	 * image segmentation based on region growing
	 */
	class RegionGrowing {

	public:

		/**
		 * get all other pixels beloging attached to the same seed.
		 * pixels count as attached when their difference is below the given threshold.
		 * @param seed the position to start searching
		 * @parma used track which points already belong to a segment and are thus skipped
		 * @param threshold the difference threshold to use
		 * @return
		 */
		static Segment<float> get(const ImageChannel& img, const Point2i& seed, Bitmap& used, const float threshold = 0.1f) {

			// track all to-be-checked points
			std::vector<Point2i> toCheck;

			// the detected region
			Segment<float> seg;
			seg.avg = 0;

			// start at the seed and mark it as used
			toCheck.push_back(seed);
			used.set(seed);
			seg.points.push_back(seed);

			// while we have something to check
			while (!toCheck.empty()) {

				// get the next element to check the neighborhood for
				const Point2i cur = toCheck.back();
				const float curVal = img.get(cur.x, cur.y);

				// update the average value
				seg.avg += curVal;

				// remove it from the to-be-checked list
				toCheck.erase(toCheck.end() - 1);

				// funny.. faster than static inline?!
				auto checkAdd = [&] (const Point2i p) {

					// skip points that belong to another segment
					if (used.isSet(p))						{return;}

					// skip above-threshold
					const float nextVal = img.get(p.x, p.y);
					const float diff = curVal - nextVal;
					if (std::abs(diff) > threshold)			{return;}

					// fine! -> add

					// adjust the segment (add the point, update average value, update the used-bitmap)
					seg.points.push_back(p);
					used.set(p);
					toCheck.push_back(p);

				};

				// reached boundaries?
				const bool l = cur.x > 0;							// there are pixels on the left
				const bool t = cur.y > 0;							// there are pixels above
				const bool r = cur.x < img.getWidth()-1;			// there are pixels on the right
				const bool b = cur.y < img.getHeight()-1;			// there are pixels below

				// check neighborhood (like a clock! order is important!)
				if (l)			{checkAdd( Point2i(cur.x-1, cur.y  ) );}
				if (l&&t)		{checkAdd( Point2i(cur.x-1, cur.y-1) );}
				if (t)			{checkAdd( Point2i(cur.x  , cur.y-1) );}
				if (r&&t)		{checkAdd( Point2i(cur.x+1, cur.y-1) );}
				if (r)			{checkAdd( Point2i(cur.x+1, cur.y  ) );}
				if (r&&b)		{checkAdd( Point2i(cur.x+1, cur.y+1) );}
				if (b)			{checkAdd( Point2i(cur.x  , cur.y+1) );}
				if (l&&b)		{checkAdd( Point2i(cur.x-1, cur.y+1) );}

			}

			// adjust the average
			seg.avg /= (float) seg.points.size();

			// done
			return seg;

		}


		/**
		 * get all other pixels beloging attached to the same seed.
		 * pixels count as attached when their difference is below the given threshold.
		 * @param seed the position to start searching
		 * @param threshold the difference threshold to use
		 * @return
		 */
		static Segment<float> get(const ImageChannel& img, const Point2i& seed, const float threshold = 0.1f) {

			// track all visited points
			Bitmap used(img.getWidth(), img.getHeight());

			// execute
			return get(img, seed, used, threshold);

		}


	private:



	};

}

#endif // REGIONGROWING_H
