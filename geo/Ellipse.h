#ifndef K_GEO_ELLIPSE_H
#define K_GEO_ELLIPSE_H

#include <vector>

#include "Point2.h"

#include <eigen3/Eigen/Dense>

namespace K {

	class Ellipse {

	public:

		struct CanonicalParams;
		struct GeometricParams;

		// https://en.wikipedia.org/wiki/Ellipse#General_ellipse
		// http://math.stackexchange.com/questions/1824910/how-to-get-the-coordinates-of-the-center-of-the-ellipse-after-approximation
		struct CanonicalParams {

			float A;
			float B;
			float C;
			float D;
			float E;
			float F;

			/** empty ctor */
			CanonicalParams() : A(1), B(1), C(1), D(1), E(1), F(1) {;}

			/** ctor */
			CanonicalParams(const float A, const float B, const float C, const float D, const float E, const float F) : A(A), B(B), C(C), D(D), E(E), F(F) {;}

			/** ctor with geometric details */
			template <typename Scalar> CanonicalParams(const K::Point2<Scalar> center, const float a, const float b, const float rad) {
				const float cosR = std::cos(rad);
				const float sinR = std::sin(rad);
				A = a*a*sinR*sinR + b*b*cosR*cosR;
				B = 2*(b*b - a*a) * sinR * cosR;
				C = a*a*cosR*cosR  + b*b*sinR*sinR;
				D = -2*A*center.x - B*center.y;
				E = -B*center.x - 2*C*center.y;
				F = A*center.x*center.x + B*center.x*center.y + C*center.y*center.y - a*a*b*b;
			}

			/** get the ellipse's center */
			K::Point2f getCenter() const {
				const float cx = (2*C*D - B*E) / (B*B - 4*A*C);
				const float cy = (2*A*E - B*D) / (B*B - 4*A*C);
				return K::Point2f(cx, cy);
			}

			/** get the ellipse's both axis lengths */
			K::Point2f getRadi() const {
				const double Fx = F + ( D*(B*E - C*D) - A*E*E ) / (4*A*C - B*B);
				const float a = (float) std::sqrt( (-2*Fx) / (A+C + std::sqrt(B*B + (A-C)*(A-C)) ) );
				const float b = (float) std::sqrt( (-2*Fx) / std::abs(A+C - std::sqrt(B*B + (A-C)*(A-C)) ) );
				return K::Point2f(b,a);
			}

			/** get the ellipse's rotation angle */
			float getAngle() const {
				const float rad = std::atan( (C - A - std::sqrt( (A-C)*(A-C) + B*B )) / B );
				return rad;
			}

			/** get the error for (x,y) not belonging to the ellipse */
			float getError(const float x, const float y) const {

				// very important constraint! otherwise error-calculation fails!
				// // https://en.wikipedia.org/wiki/Ellipse#General_ellipse
				if ((B*B - 4*A*C) >= 0) {return NAN;}

				return A*x*x + B*x*y + C*y*y + D*x + E*y + F;
				//return A*x*x + B*x*y + C*y*y + fo*(D*x + E*y) + fo*fo*F;

			}

			void normalize() {
				const float v = A*A + B*B + C*C + D*D + E*E + F*F;
				A/=v; B/=v; C/=v, D/=v, E/=v, F/=v;
			}


			float getErrorSampson(const float x, const float y) const {

//				Eigen::Matrix<float, 6, 1> xi; xi << x*x, x*y, y*y, x, y, 1;
//				Eigen::Matrix<float, 6, 1> theta; theta << A,B,C,D,E,F;
//				Eigen::Matrix<float, 6, 6> covar; covar <<
//					x*x,	x*y,	0,		x,		0,		0,
//					x*y,	x*x+y*y,x*y,	y,		x,		0,
//					0,		x*y,	y*y,	0,		y,		0,
//					x,		y,		0,		1,		0,		0,
//					0,		x,		y,		0,		1,		0,
//					0,		0,		0,		0,		0,		0;
//				covar = 4*covar;

////				std::cout << xi << std::endl << std::endl;
////				std::cout << theta << std::endl << std::endl;
////				std::cout << covar << std::endl << std::endl;

//				const float a = xi.dot(theta);				// same as "getError()"
//				const float b = theta.dot(covar*theta);
//				return a*a/b;

				Eigen::Matrix<double, 3, 1> X; X << x, y, 1;
				Eigen::Matrix<double, 3, 3> Q; Q <<
					A,B,D,
					B,C,E,
					D,E,F;
				Eigen::Matrix<double, 3, 3> P; P <<
					1,0,0,
					0,1,0,
					0,0,0;

				const double a = X.dot(Q*X);
				const double b = (Q*X).dot(P*Q*X);
				const double res = a/(4*b);
				return (float)res;


			}

			/** convert to geometric parameters */
			GeometricParams toGeometric() const;


		};



		/**
		 * geometric ellipse description:
		 * center
		 * radius a/b
		 * rotation
		 */
		struct GeometricParams {

			K::Point2f center;

			float a;
			float b;

			float rad;

			/** empty ctor */
			GeometricParams() {;}

			/** ctor */
			GeometricParams(const K::Point2f center, const float a, const float b, const float rad) : center(center), a(a), b(b), rad(rad) {;}

			/** get a point on this ellipse for the position identified by rad [0:2PI] */
			K::Point2f getPointFor(const float rad) const {

				// http://stackoverflow.com/questions/2153768/draw-ellipse-and-ellipsoid-in-matlab

				const float cosA = std::cos(rad);			// the point on the circle to draw
				const float sinA = std::sin(rad);

				const float cosB = std::cos(this->rad);		// rotation of the ellipse
				const float sinB = std::sin(this->rad);

				const float x = center.x + (a * cosA * cosB - b * sinA * sinB);
				const float y = center.y + (a * cosA * sinB + b * sinA * cosB);

				return K::Point2f(x,y);

			}

			/** APX circumfence */
			float getCircumfence() const {
				const float h = ((a-b)*(a-b)) / ((a+b)*(a+b));
				return M_PI * (a+b) * ( 1 + (3*h) / (10+std::sqrt(4-3*h)) );
			}

			/** convert to canonical parameters */
			CanonicalParams toCanonical() const;

		};


	};

	inline Ellipse::GeometricParams Ellipse::CanonicalParams::toGeometric() const {
		return GeometricParams(getCenter(), getRadi().x, getRadi().y, getAngle());
	}

	inline Ellipse::CanonicalParams Ellipse::GeometricParams::toCanonical() const {
		return CanonicalParams(center, a, b, rad);
	}

}

#endif // K_GEO_ELLIPSE_H
