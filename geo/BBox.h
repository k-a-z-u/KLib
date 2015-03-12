#ifndef K_GEO_BBOX_H
#define K_GEO_BBOX_H

#include <limits>
#include "Point.h"

/**
 * this class represents an axis-aligned 2D bounding box
 */
namespace K {

	class BBox {

		static constexpr double _MAX = std::numeric_limits<double>::max();
		static constexpr double _MIN = std::numeric_limits<double>::min();

	public:

		/** empty ctor */
		BBox() : pMin(_MAX, _MAX), pMax(_MIN, _MIN) {
			;
		}

		/** ctor */
		BBox (const Point& pMin, const Point& pMax) :
			pMin(pMin), pMax(pMax) {
			;
		}


		/** "resize" the bounding-box by adding the given point */
		void add(const Point& p) {

			if (p.x < pMin.x) {pMin.x = p.x;}
			if (p.y < pMin.y) {pMin.y = p.y;}

			if (p.x > pMax.x) {pMax.x = p.x;}
			if (p.y > pMax.y) {pMax.y = p.y;}

		}

		/** add the given bbox to this bbox */
		void add(const BBox& o) {
			add(o.pMin);
			add(o.pMax);
		}

		/** does the bounding-box contain the given point? */
		bool contains(const Point& p) const {

			return	(pMin.x <= p.x) &&
					(pMin.y <= p.y) &&
					(pMax.x >= p.x) &&
					(pMax.y >= p.y);

		}

		/** does the bouding-box intersect with the given bounding-box? */
		bool intersects(const BBox& o) const {
			bool n=	(pMax.x < o.pMin.x) ||	// left of
					(pMax.y < o.pMin.y) ||	// below
					(pMin.x > o.pMax.x) ||	// right of
					(pMin.y > o.pMax.y);	// above
			return !n;
		}

		/** equality check */
		bool operator == (const BBox& o) const {
			return (pMin == o.pMin) && (pMax == o.pMax);
		}

		/** printing */
		friend std::ostream& operator << (std::ostream& out, const BBox& bbox) {
			std::cout << "BBox\t" << bbox.pMin << " -> \t" << bbox.pMax;
			return out;
		}

		/** get this bounding box's center */
		Point getCenter() const {
			return (pMin + pMax) / 2.0f;
		}

		/** scale the bounding-box by using the given factor */
		BBox getScaled(const float fact) const {

			Point center = getCenter();
			Point nMin = (pMin - center) * fact + center;
			Point nMax = (pMax - center) * fact + center;
			return BBox(nMin, nMax);

		}

		/** get the upper-left edge of the bounding box */
		Point getMin() const {return pMin;}

		/** get the lower-right edge of the bounding box */
		Point getMax() const {return pMax;}

	private:

		/** the lower (x,y) */
		Point pMin;

		/** the higher (x,y) */
		Point pMax;

	};

}

#endif // K_GEO_BBOX_H
