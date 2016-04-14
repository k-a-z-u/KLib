#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point3.h"

namespace K {

	/** triangle in 3D space */
	template <typename T> class Triangle3 {

	private:

	using P3 = Point3<T>;

		struct ST {
			float s;
			float t;
			ST(const float s, const float t) : s(s), t(t) {;}
		};

	public:

		P3 p1;
		P3 p2;
		P3 p3;

	public:

		/** ctor */
		Triangle3(const Point3<T> p1, const Point3<T> p2, const Point3<T> p3) : p1(p1), p2(p2), p3(p3) {;}

		/** get the intersection of the triangle's normal and the given point */
		Point3<T> getIntersection(const Point3<T>& P) const {

			const P3 B = p1;		// base
			const P3 E0 = p2-p1;	// 1st vector
			const P3 E1 = p3-p1;	// 2nd vector
			ST st = getIntersectionST(P);
			return B + (E0*st.s) + (E1*st.t);

		}

		/**
		 * get the distance between the given point and the triangle
		 * giving a delta-factor [0.0:0.5] allows intersections sligthly
		 * outside of the triangle!
		 */
		float getDistance(const Point3<T>& P, const float delta = 0) const {

			const P3 B = p1;		// base
			const P3 E0 = p2-p1;	// 1st vector
			const P3 E1 = p3-p1;	// 2nd vector
			ST st = getIntersectionST(P);

			if (st.s >= -delta && st.t >= -delta && (st.s+st.t) <= 1.0+delta) {
				P3 cut = B + (E0*st.s) + (E1*st.t);
				return cut.getDistance(P);
			} else {
				return INFINITY;
			}

		}



		/** get the multiple s,t of s*E0 and t*E1 where the triangles normal intersects with P */
		ST getIntersectionST(const Point3<T>& P) const {

			// http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf

			const P3 B = p1;		// base
			const P3 E0 = p2-p1;	// 1st vector
			const P3 E1 = p3-p1;	// 2nd vector

			const P3 D = B - P;
			const float a = E0.dot(E0);
			const float b = E0.dot(E1);
			const float c = E1.dot(E1);
			const float d = E0.dot(D);
			const float e = E1.dot(D);

			const float det = a*c - b*b;
			const float s = b*e - c*d;
			const float t = b*d - a*e;

			// s,t are [0:det] to get them into [0:1] (if within the triangle), divide them by the determinant
			return ST(s/det, t/det);

		}

	};

	typedef Triangle3<float> Triangle3f;
	typedef Triangle3<int> Triangle3i;

}

#endif // TRIANGLE_H
