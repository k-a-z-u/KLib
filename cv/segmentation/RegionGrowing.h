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

			// start at the seed
			toCheck.push_back(seed);
			used.set(seed);

			// while we have something to check
			while (!toCheck.empty()) {

				// get the next element to check the neighborhood for
				const Point2i next = toCheck.back();
				toCheck.erase(toCheck.end() - 1);
				const float val = img.get(next.x, next.y);

				// adjust the region (add point, update average value)
				seg.points.push_back(next);
				seg.avg += val;

				// reached boundaries?
				const bool l = next.x > 0;							// there are pixels on the left
				const bool t = next.y > 0;							// there are pixels above
				const bool r = next.x < img.getWidth()-1;			// there are pixels on the right
				const bool b = next.y < img.getHeight()-1;			// there are pixels below

				// funny.. faster than static inline?!
				auto checkAdd = [&] (const Point2i p) {

					// skip already visisted
					if (used.isSet(p))						{return;}

					// skip above-threshold
					const float diff = val - img.get(p.x, p.y);
					if (std::abs(diff) > threshold)			{return;}

					// fine! -> add
					toCheck.push_back(p);
					used.set(p);

				};

				// check neighborhood
				if (l)			{checkAdd( Point2i(next.x-1, next.y  ) );}
				if (t)			{checkAdd( Point2i(next.x  , next.y-1) );}
				if (r)			{checkAdd( Point2i(next.x+1, next.y  ) );}
				if (b)			{checkAdd( Point2i(next.x  , next.y+1) );}

				if (l&&t)		{checkAdd( Point2i(next.x-1, next.y-1) );}
				if (l&&b)		{checkAdd( Point2i(next.x-1, next.y+1) );}
				if (r&&t)		{checkAdd( Point2i(next.x+1, next.y-1) );}
				if (r&&b)		{checkAdd( Point2i(next.x+1, next.y+1) );}

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
