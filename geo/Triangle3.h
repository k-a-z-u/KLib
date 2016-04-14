#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point3.h"

namespace K {

	/** triangle in 3D space */
	template <typename T> class Triangle3 {

	using P3 = Point3<T>;

	private:

		P3 p1;
		P3 p2;
		P3 p3;

	public:

		/** ctor */
		Triangle3(const Point3<T> p1, const Point3<T> p2, const Point3<T> p3) : p1(p1), p2(p2), p3(p3) {;}

		/** get the distance between the given point and the triangle */
		float getDistance(const Point3<T>& P) const {

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
			//const float f = D.dot(D);

			const float det = a*c - b*b;
			const float s = b*e - c*d;
			const float t = b*d - a*e;

			// s,t are [0:det]
			// to get them into [0:1], divide them by the determinant
			if (s+t <= det && s >= 0 && t >= 0) {
				int i = 0; (void) i;
				P3 cut = B + (E0*s/det) + (E1*t/det);
				return cut.getDistance(P);
			} else {
				return INFINITY;
			}

		}


	};

	typedef Triangle3<float> Triangle3f;
	typedef Triangle3<int> Triangle3i;

}

#endif // TRIANGLE_H
