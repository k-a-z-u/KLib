#ifndef SEGMENT_H
#define SEGMENT_H

#include <cmath>
#include <vector>

#include "../ImageChannel.h"
#include "../Point2.h"
#include "../BBox2.h"

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


		/** calculate the segment's bbox */
		BBox2i calcBBox() const {
			BBox2i bb;
			for (const Point2i p : points) {bb.add(p);}
			return bb;
		}

		/** get the segment as a b/w image */
		ImageChannel asImage(const bool segIsWhite = true) const {

			// calculate the segments's bounding-box
			const BBox2i bb = calcBBox();

			// create and fill output image
			ImageChannel img(bb.getWidth()+1, bb.getHeight()+1);
			if (segIsWhite) {img.zero();} else {img.ones();}

			// add points
			for (const Point2i& _p : points) {
				const Point2i p = _p - bb.getMin();
				img.set( p.x, p.y, ((segIsWhite) ? (1) : (0)) );
			}

			// done
			return img;

		}

	};

}

#endif // SEGMENT_H
