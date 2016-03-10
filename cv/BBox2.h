#ifndef K_CV_BBOX2_H
#define K_CV_BBOX2_H

#include "Point2.h"

/**
 * this class represents an axis-aligned 2D bounding box
 */
namespace K {

	template <typename T> class BBox2 {

		static constexpr T _MAX = std::numeric_limits<T>::max();
		static constexpr T _MIN = std::numeric_limits<T>::min();

	private:

		Point2<T> pMin;
		Point2<T> pMax;

	public:

		/** empty ctor */
		BBox2() : pMin(_MAX, _MAX), pMax(_MIN, _MIN) {
			;
		}

		/** ctor */
		BBox2 (const Point2<T>& pMin, const Point2<T>& pMax) :
			pMin(pMin), pMax(pMax) {
			;
		}


		/** "resize" the bounding-box by adding the given point */
		void add(const Point2<T>& p) {

			if (p.x < pMin.x) {pMin.x = p.x;}
			if (p.y < pMin.y) {pMin.y = p.y;}

			if (p.x > pMax.x) {pMax.x = p.x;}
			if (p.y > pMax.y) {pMax.y = p.y;}

		}

		/** add the given bbox to this bbox */
		void add(const BBox2<T>& o) {
			add(o.pMin);
			add(o.pMax);
		}

		/** does the bounding-box contain the given point? */
		bool contains(const Point2<T>& p) const {

			return	(pMin.x <= p.x) &&
					(pMin.y <= p.y) &&
					(pMax.x >= p.x) &&
					(pMax.y >= p.y);

		}

		/** does the bouding-box intersect with the given bounding-box? */
		bool intersects(const BBox2<T>& o) const {
			bool n=	(pMax.x < o.pMin.x) ||	// left of
					(pMax.y < o.pMin.y) ||	// below
					(pMin.x > o.pMax.x) ||	// right of
					(pMin.y > o.pMax.y);	// above
			return !n;
		}

		/** equality check */
		bool operator == (const BBox2<T>& o) const {
			return (pMin == o.pMin) && (pMax == o.pMax);
		}

		/** get the upper-left edge of the bounding box */
		Point2<T> getMin() const {return pMin;}

		/** get the lower-right edge of the bounding box */
		Point2<T> getMax() const {return pMax;}

		T getWidth() const {return pMax.x - pMin.x;}
		T getHeight() const {return pMax.y - pMin.y;}

	};

	typedef BBox2<float> BBox2f;
	typedef BBox2<int> BBox2i;

}

#endif // K_CV_BBOX2_H
