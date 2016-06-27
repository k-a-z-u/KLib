#ifndef BBOX3_H
#define BBOX3_H

#include "Point3.h"

/**
 * this class represents an axis-aligned 3D bounding box
 */
namespace K {

	template <typename T> class BBox3 {

		static constexpr T _MAX = std::numeric_limits<T>::max();
		static constexpr T _MIN = std::numeric_limits<T>::min();

	private:

		Point3<T> pMin;
		Point3<T> pMax;

	public:

		/** empty ctor */
		BBox3() : pMin(_MAX, _MAX, _MAX), pMax(_MIN, _MIN, _MIN) {
			;
		}

		/** ctor */
		BBox3 (const Point3<T>& pMin, const Point3<T>& pMax) :
			pMin(pMin), pMax(pMax) {
			;
		}


		/** "resize" the bounding-box by adding the given point */
		void add(const Point3<T>& p) {

			if (p.x < pMin.x) {pMin.x = p.x;}
			if (p.y < pMin.y) {pMin.y = p.y;}
			if (p.z < pMin.z) {pMin.z = p.z;}

			if (p.x > pMax.x) {pMax.x = p.x;}
			if (p.y > pMax.y) {pMax.y = p.y;}
			if (p.z > pMax.z) {pMax.z = p.z;}

		}

		/** grow the bounding-box by the given factor (0.1 = +5% in each direction) */
		void growFactor(const float factor) {
			const T aw = (T) (getWidth() * (factor/2));
			const T ah = (T) (getHeight() * (factor/2));
			const T ad = (T) (getDepth() * (factor/2));
			pMin.x -= aw;
			pMin.y -= ah;
			pMin.z -= ad;
			pMax.x += aw;
			pMax.y += ah;
			pMax.z += ad;
		}

		/** grow the bounding-box by the value into each direction */
		void growValue(const T value) {
			pMin.x -= value;
			pMin.y -= value;
			pMin.z -= value;
			pMax.x += value;
			pMax.y += value;
			pMax.z += value;
		}


		/** add the given bbox to this bbox */
		void add(const BBox3<T>& o) {
			add(o.pMin);
			add(o.pMax);
		}

		/** does the bounding-box contain the given point? */
		bool contains(const Point3<T>& p) const {

			return	(pMin.x <= p.x) &&
					(pMin.y <= p.y) &&
					(pMin.z <= p.z) &&
					(pMax.x >= p.x) &&
					(pMax.y >= p.y) &&
					(pMax.z >= p.z);

		}

		/** does the bounding-box fully contain the given bounding box? */
		bool contains(const BBox3<T>& b) const {

			return	(pMin.x < b.pMin.x) &&
					(pMin.y < b.pMin.y) &&
					(pMin.z < b.pMin.z) &&
					(pMax.x > b.pMax.x) &&
					(pMax.y > b.pMax.y) &&
					(pMax.z > b.pMax.z);

		}

		/** does the bounding-box intersect with the given bounding box? */
		bool intersects(const BBox3<T>& b) const {

			return	(pMin.x <= b.pMax.x) &&
					(pMin.y <= b.pMax.y) &&
					(pMin.z <= b.pMax.z) &&
					(pMax.x >= b.pMin.x) &&
					(pMax.y >= b.pMin.y) &&
					(pMax.z >= b.pMin.z);

		}

		/** genter the BBox at (0,0) */
		void center() {
			const Point3<T> size = pMax-pMin;
			pMin = -size/2;
			pMax =  size/2;
		}

		/** move the bbox by the given amount */
		void moveBy(const Point3<T> pos) {
			pMin += pos;
			pMax += pos;
		}

		/** equality check */
		bool operator == (const BBox3<T>& o) const {
			return (pMin == o.pMin) && (pMax == o.pMax);
		}

		/** get the upper-left edge of the bounding box */
		Point3<T> getMin() const {return pMin;}

		/** get the lower-right edge of the bounding box */
		Point3<T> getMax() const {return pMax;}

		/** get the BBox's center */
		Point3<T> getCenter() const {return (pMin + pMax)/2;}

		T getWidth() const {return pMax.x - pMin.x;}
		T getHeight() const {return pMax.y - pMin.y;}
		T getDepth() const {return pMax.z - pMin.z;}

	};

	typedef BBox3<float> BBox3f;
	typedef BBox3<int> BBox3i;

}

#endif // BBOX3_H
