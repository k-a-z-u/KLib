#ifndef CIRCLE3_H
#define CIRCLE3_H

#include "Point3.h"
#include "../geo/Matrix.h"
#include "../Assertions.h"

namespace K {

	/**
	 * 3D circle
	 */
	template <typename Scalar> struct Circle3 {

		/** the circle's center */
		Point3<Scalar> center;

		/** the circle's normal (axis) */
		Point3<Scalar> normal;

		/** the circle's radius */
		Scalar radius;


		/** empty ctor */
		Circle3() : center(), normal(), radius(0) {
			;
		}


		/** ctor */
		Circle3(const Point3<Scalar>& center, const Point3<Scalar>& normal, const Scalar radius) : center(center), normal(normal.normalized()), radius(radius) {

			// some sanity checks
			_assertNear(1, this->normal.getLength(), 0.01, "length of the normal must be 1");
			_assertTrue(this->radius > 0, "radius must be > 0");

		}

		/** get the point on the circle for the given radians */
		const K::Point3<Scalar> getPointFor(const float rad) const {

			// 2D (x,y)
			const float x = std::cos(rad) * radius;
			const float y = std::sin(rad) * radius;

			// TODO: SLOW!
			// calculate rotation from (0,0,1)-normal to the target-normal
			Eigen::Vector3f from; from << 0, 0, 1;
			Eigen::Vector3f to; to << normal.x, normal.y, normal.z;
			const Eigen::Matrix3f rotMat = MatrixHelper::getRotationMatrix(from, to);

			// perform rotation and add the center-point
			const Point3<Scalar> p2(x,y,0);
			const Point3<Scalar> p3 = (rotMat * p2) + center;

			return p3;

		}

		/** get the given point's distance from the circle's outline */
		const Scalar getDistanceFromOutline(const K::Point3f p) const {
			return std::abs(center.getDistance(p) - radius);
		}


	};

	typedef Circle3<float> Circle3f;
	typedef Circle3<int> Circle3i;

}

#endif // CIRCLE3_H
