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

		/** ctor */
		BBox2 (const T minX, const T minY, const T maxX, const T maxY) :
			pMin(minX, minY), pMax(maxX, maxY) {
			;
		}

		/** clear/reset the BBox */
		void clear() {
			pMin = Point2<T>(_MAX, _MAX);
			pMax = Point2<T>(_MIN, _MIN);
		}

		/** adjust the bounding-box by adding the given point */
		void add(const T x, const T y) {

			if (x < pMin.x) {pMin.x = x;}
			if (y < pMin.y) {pMin.y = y;}

			if (x > pMax.x) {pMax.x = x;}
			if (y > pMax.y) {pMax.y = y;}

		}

		/** adjust the bounding-box by adding the given point */
		void add(const Point2<T>& p) {
			add(p.x, p.y);
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
		bool contains(const T x, const T y) const {
			return	(pMin.x <= x) &&
					(pMin.y <= y) &&
					(pMax.x >= x) &&
					(pMax.y >= y);
		}

		/** does the bounding-box contain the given point? */
		bool contains(const Point2<T>& p) const {
			return contains(p.x, p.y);
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

		/** genter the BBox at (0,0) */
		void center() {
			const Point2<T> size = pMax-pMin;
			pMin = -size/2;
			pMax =  size/2;
		}

		/** move the bbox by the given amount */
		void moveBy(const Point2<T> pos) {
			pMin += pos;
			pMax += pos;
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


		/** get the intersection between this bbox and the given bbox */
		BBox2<T> intersection(const BBox2<T>& o) const {
			const Point2<T> min( std::max(pMin.x, o.pMin.x), std::max(pMin.y, o.pMin.y) );
			const Point2<T> max( std::min(pMax.x, o.pMax.x), std::min(pMax.y, o.pMax.y) );
			return BBox2<T>(min, max);
		}

		T getWidth() const {return pMax.x - pMin.x;}
		T getHeight() const {return pMax.y - pMin.y;}

	};



	typedef BBox2<float> BBox2f;
	typedef BBox2<int> BBox2i;

}

#endif // K_CV_BBOX2_H
