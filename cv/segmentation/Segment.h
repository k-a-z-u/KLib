#ifndef SEGMENT_H
#define SEGMENT_H

#include <cmath>
#include <vector>
#include "../Point2.h"

namespace K {

	/**
	 * describes one region (segment) detected within an image
	 */
	struct Segment {

		/** all points belonging to the segment */
		std::vector<Point2i> points;

		/** the average value for the segment */
		float avg;

		/** ctor */
		Segment() : points(), avg(NAN) {;}

	};

}

#endif // SEGMENT_H
