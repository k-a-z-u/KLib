#ifndef K_CV_BBOX2_H
#define K_CV_BBOX2_H

#include "../geo/Point2.h"

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

		/** grow the bounding-box by the given factor (0.1 = +5% in each direction) */
		void growFactor(const float factor) {
			const T aw = (T) (getWidth() * (factor/2));
			const T ah = (T) (getHeight() * (factor/2));
			pMin.x -= aw;
			pMin.y -= ah;
			pMax.x += aw;
			pMax.y += ah;
		}

		/** grow the bounding-box by the value into each direction */
		void growValue(const T value) {
			pMin.x -= value;
			pMin.y -= value;
			pMax.x += value;
			pMax.y += value;
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

		/** does the bounding-box fully contain the given bounding box? */
		bool contains(const BBox2<T>& b) const {

			return	(pMin.x < b.pMin.x) &&
					(pMin.y < b.pMin.y) &&
					(pMax.x > b.pMax.x) &&
					(pMax.y > b.pMax.y);

		}

		/** does the bounding-box intersect with the given bounding box? */
		bool intersects(const BBox2<T>& b) const {

			return	(pMin.x <= b.pMax.x) &&
					(pMin.y <= b.pMax.y) &&
					(pMax.x >= b.pMin.x) &&
					(pMax.y >= b.pMin.y);

		}

//		/** does the bouding-box intersect with the given bounding-box? */
//		bool intersects(const BBox2<T>& o) const {
//			return =	(pMax.x < o.pMin.x) ||	// left of
//					(pMax.y < o.pMin.y) ||	// below
//					(pMin.x > o.pMax.x) ||	// right of
//					(pMin.y > o.pMax.y);	// above
//			return !n;
//		}

		/** equality check */
		bool operator == (const BBox2<T>& o) const {
			return (pMin == o.pMin) && (pMax == o.pMax);
		}

		/** get the upper-left edge of the bounding box */
		Point2<T> getMin() const {return pMin;}

		/** get the lower-right edge of the bounding box */
		Point2<T> getMax() const {return pMax;}

		/** get the BBox's center */
		Point2<T> getCenter() const {return (pMin + pMax)/2;}

		T getWidth() const {return pMax.x - pMin.x;}
		T getHeight() const {return pMax.y - pMin.y;}

	};

	typedef BBox2<float> BBox2f;
	typedef BBox2<int> BBox2i;

}

#endif // K_CV_BBOX2_H
