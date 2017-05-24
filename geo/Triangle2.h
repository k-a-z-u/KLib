#ifndef TRIANGLE2_H
#define TRIANGLE2_H

#include "Point2.h"
#include "BBox2.h"

namespace K {

	template <typename T> class Triangle2 {

	public:

		Point2<T> p1;
		Point2<T> p2;
		Point2<T> p3;

	public:

		/** ctor */
		Triangle2(const Point2<T> p1, const Point2<T> p2, const Point2<T> p3) : p1(p1), p2(p2), p3(p3) {
			;
		}

		/** does the triangle contain the given point? */
		bool contains(const Point2<T> p) const {

			// total area
			const float A = area(p1, p2, p3);

			// area between edges and points
			const float A1 = area(p, p2, p3);
			const float A2 = area(p1, p, p3);
			const float A3 = area(p1, p2, p);

			const float err = A - (A1 + A2 + A3);
			return err < A*0.001f;

		}

		/** does the triangle contain the given point? */
		bool contains(const Point2<T> p, float& u, float& v, float& w) const {

			// total area
			const float A = area(p1, p2, p3);

			// area between edges and points
			const float A1 = area(p, p2, p3);
			const float A2 = area(p1, p, p3);
			const float A3 = area(p1, p2, p);

			u = A1/A;
			v = A2/A;
			w = A3/A;

			const float err = std::abs( A - (A1 + A2 + A3) );
			return err < A*0.001f;

		}

		/** get the triangle's bounding-box */
		BBox2<T> getBBox() const {
			BBox2<T> bb;
			bb.add(p1);
			bb.add(p2);
			bb.add(p3);
			return bb;
		}

	private:

		inline float area(const Point2<T> p1, const Point2<T> p2, const Point2<T> p3) const {
			return area(p1.x,p1.y, p2.x,p2.y, p3.x,p3.y);
		}

		inline float area(const T x1, const T y1, const T x2, const T y2, const T x3, const T y3) const {
			return std::abs( (x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)) / 2.0f );
		}

	};

	typedef Triangle2<float> Triangle2f;
	typedef Triangle2<int> Triangle2i;


}

#endif // TRIANGLE2_H
