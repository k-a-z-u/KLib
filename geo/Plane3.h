#ifndef K_GEO_PLANE3_H
#define K_GEO_PLANE3_H

#include "Point3.h"
#include "../Assertions.h"

namespace K {

	/**
	 * 3D plane
	 */
	template <typename Scalar> struct Plane3 {

		struct CoordinateParams;

		/** coordinate description: normal-vector + distance from (0,0,0) */
		struct CoordinateParams {

			/** the plain's normal */
			Point3<Scalar> normal;

			/** the plain's distance from (0,0,0) */
			Scalar distance;


			/** empty ctor */
			CoordinateParams() : normal(), distance(0) {
				;
			}

			/** ctor */
			CoordinateParams(const Point3<Scalar>& normal, const Scalar distance) : normal(normal), distance(distance) {

				_assertNear(1, normal.getLength(), 0.01, "normal must be normalized!");

			}

			/** get the distance between the plane and the given point */
			Scalar getDistance(const Point3<Scalar>& p) const {

				// http://mathworld.wolfram.com/Point-PlaneDistance.html
				const Scalar d =	( (normal.x * p.x) + (normal.y * p.y) + (normal.z * p.z) - distance ) /
									normal.getLength();

				return std::abs(d);

			}

		};

		struct NormalParams {

			/** the plain's normal */
			Point3<Scalar> normal;

			/** the plain's normal */
			Point3<Scalar> center;

			/** ctor */
			NormalParams(const Point3<Scalar>& normal, const Point3<Scalar>& center) : normal(normal), center(center) {
				;
			}

		};

	};

	typedef Plane3<float> Plane3f;
	typedef Plane3<int> Plane3i;


}

#endif // K_GEO_PLANE3_H
