#ifndef K_GEO_CIRCLE2_H
#define K_GEO_CIRCLE2_H

#include "../Assertions.h"
#include "Point2.h"

namespace K {

	/**
	 * 2D circle
	 */
	template <typename Scalar> struct Circle2 {


	public:

		struct CanonicalParams;

		// http://www.qc.edu.hk/math/Advanced%20Level/circle%20given%203%20points.htm
		// http://www.mathsisfun.com/algebra/circle-equations.html
		// http://www.intmath.com/plane-analytic-geometry/3-circle.php
		struct CanonicalParams {

			Scalar D;
			Scalar E;
			Scalar F;

			/** empty ctor */
			CanonicalParams() : D(0), E(0), F(1) {
				;
			}

			/** ctor */
			CanonicalParams(const float D, const float E, const float F) : D(D), E(E), F(F) {
				// NO!!
//				if (F > 0) {
//					this->D = -D;
//					this->E = -E;
//					this->F = -F;
//				}
			}

			/** is this a valid circle? */
			bool valid() const {
				return (D+E) != 0;	// see empty ctor
			}

//			/** get the error for (x,y) not belonging to the circle */
//			float getError(const float x, const float y) const {
//				return x*x + y*y + D*x + E*y + F;
//			}


			/** get the geometric radius */
			float getRadius() const {
				return std::sqrt( (D/2)*(D/2) + (E/2)*(E/2) - F );
			}

			/** get the geometric center */
			Point2<Scalar> getCenter() const {
				return Point2<Scalar>( -(D/2), -(E/2) );
			}

			/** convert to a geometric representation */
			Circle2<Scalar> toGeometric() const;

		};





		/** the circle's center */
		Point2<Scalar> center;

		/** the circle's radius */
		float radius;


		/** ctor */
		Circle2(const Point2<Scalar> center, const float radius) : center(center), radius(radius) {

			// sanity checks
			_assertNotNAN(radius, "radius is NaN");
			_assertNotNAN(center.getLength(), "center has NaN values");

		}

		/** convert radians to a position (x,y) on the circle */
		Point2<Scalar> getPointFor(const float rad) const {
			const Scalar x = std::cos(rad) * radius;
			const Scalar y = std::sin(rad) * radius;
			return center + Point2<Scalar>(x,y);
		}

		/** get the distance between the given point and the circle's outline */
		const Scalar getDistanceFromOutline(const Point2<Scalar> p) const {
			return std::abs(p.getDistance(center) - radius);
		}





	};

	template <typename Scalar> Circle2<Scalar> Circle2<Scalar>::CanonicalParams::toGeometric() const {
		return Circle2(getCenter(), getRadius());
	}

	typedef Circle2<float> Circle2f;
	typedef Circle2<int> Circle2i;


}

#endif // K_GEO_CIRCLE2_H
