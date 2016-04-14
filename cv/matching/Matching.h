#ifndef MATCHING_H
#define MATCHING_H

#include "../../geo/Point2.h"
#include "../ImageChannel.h"

namespace K {

	class Matching {

	public:

		/** refine the hint p2, to better match the given p1 */
		template <typename Matcher> static Point2i refine(const Matcher& m, const Point2i p1, const Point2i p2) {

			int s = 5;

			Point2i min;
			float minErr = INFINITY;

			Point2i o(0,0);
			for (o.y = -s; o.y <= +s; ++o.y) {
				for (o.x = -s; o.x <= +s; ++o.x) {

					const float err = m.getError(p1, p2 + o);
					if (err < minErr) {minErr = err; min = o;}

				}
			}

			return p2+min;

		}


	};

}

#endif // MATCHING_H
