#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point3.h"

#include <eigen3/Eigen/Dense>

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
		 * get the distance between the given point and the triangle (perpendicular)
		 * giving a delta-factor [0.0:0.5] allows intersections sligthly
		 * outside of the triangle!
		 */
		float getDistancePerp(const Point3<T>& P, const float delta = 0) const {

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

		struct D {
			Point3<T> isect;
			float dist;
			D(Point3<T> isect, float dist) : isect(isect), dist(dist) {;}
		};



		std::vector<P3> pts;

		/**
		 * get the minimum distance betwenn the given point and the triangle
		 */
		D getDistanceAny(const Point3<T>& p) const {

//			if (pts.empty()) {

//				const P3 B = p1;		// base
//				const P3 E0 = p2-p1;	// 1st vector
//				const P3 E1 = p3-p1;	// 2nd vector

//				for (int i = 0; i < 10; ++i) {
//					float r1 = rand() / (float) RAND_MAX / 2;
//					float r2 = rand() / (float) RAND_MAX / 2;
//					P3 np = B + (E0*r1) + (E1*r2);
//					((std::vector<P3>&)pts).push_back(np);
//				}

//			}

//			auto comp = [&] (const P3& a, const P3& b) {return a.getDistance(p) < b.getDistance(p);};
//			auto it = std::min_element(pts.begin(), pts.end(), comp);
//			P3 res = *it;

//			return D(res, res.getDistance(p));


			const K::Point3<T> A = p1;
			const K::Point3<T> B = (p2-p1);
			const K::Point3<T> C = (p3-p1);

			Eigen::Matrix<float,5,1> res; res << (p.x - A.x), (p.y - A.y), (p.z - A.z), 1, 0;

			Eigen::Matrix<float,5,2> mat; mat <<
				(float)B.x, (float)C.x,
				(float)B.y, (float)C.y,
				(float)B.z, (float)C.z,
				1,			1,
				1,			1;

			auto mati = (mat.transpose() * mat).inverse() * mat.transpose();

			Eigen::Vector2f unknown = mati * res;

			const float norm = std::abs(unknown(0)) + std::abs(unknown(1));
			if (norm > 1) {	unknown /= norm; }

			if (unknown(0) < 0) {unknown(0) = 0;}
			if (unknown(1) < 0) {unknown(1) = 0;}

			if (unknown(0) > 1) {unknown(0) = 1;}
			if (unknown(1) > 1) {unknown(1) = 1;}

			const Point3<T> xx = A + (B*unknown(0)) + (C*unknown(1));
			const float dist = xx.getDistance(p);
			return D(xx, dist);

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
