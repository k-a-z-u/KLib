#ifndef K_GEO_POLYGON_H
#define K_GEO_POLYGON_H

#include <vector>

#include "Point.h"
#include "Line.h"
#include "BBox.h"

#include "../Assertions.h"

/**
 * this class represents a polygon in 2D space and
 * provides commen helper methods.
 */
namespace K {

	class Polygon {

	public:

		/** add a new point to the polygon */
		void add(const Point& p) {
			points.push_back(p);
			bb.add(p);
		}

		/** get all points within the polygon */
		const std::vector<Point>& getPoints() const {
			return points;
		}

		/** close the polygon */
		void close() {
			_assertFalse(points.empty(), "can not close an empty polygon!");
			points.push_back(points.front());
		}

		/** get the polygon's bound box */
		const BBox& getBBox() const {return bb;}

		/** get the area enclosed by this polygon */
		double getArea() const {
			double sum = 0;
			const size_t size = points.size();
			for (size_t i = 0; i < size+1; ++i) {
				const Point& p1 = points[ (i+0)        ];
				const Point& p2 = points[ (i+1) % size ];
				sum += (p1.x * p2.y) - (p1.y * p2.x);
			}
			return sum / 2.0;
		}

		/** does the polygon contain the given point? */
		bool contains(const Point& p) const {

			// not within bbox? -> not within polygon!
			if (!bb.contains(p)) {return false;}

			// intersect a ray with each side of the polygon
			// ray starts outside the polygon and heads to the point to check
			// outside = min of the bounding-box minus some random number:
			static constexpr double rndX = 3.230345;
			static constexpr double rndY = 2.190234;
			const Line ray = Line(bb.getMin().x - rndX, bb.getMin().y - rndY, p.x, p.y);

			// intersect ray-segment with every segment of the polygon
			uint32_t numIntersections = 0;
			const size_t size = points.size();
			Point dummy;
			for (uint32_t i = 0; i < size; ++i) {
				const Point& p1 = points[(i+0)     ];
				const Point& p2 = points[(i+1)%size];
				const Line lPoly(p1, p2);
				if (ray.getSegmentIntersection(lPoly, dummy)) {++numIntersections;}
			}

			// if odd number of intersections -> inside polygon
			return (numIntersections %2) == 1;


		}


	private:

		/** all points within the polygon */
		std::vector<Point> points;

		/** the polygons bounding box */
		BBox bb;

	};

}

#endif // K_GEO_POLYGON_H
