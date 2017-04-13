#ifndef K_CV_CIRCLEDETECTION2_H
#define K_CV_CIRCLEDETECTION2_H


#include <eigen3/Eigen/Dense>

#include "../../geo/Circle2.h"
#include "../../Assertions.h"

namespace K {


	template <typename Scalar> struct CircleEstimator2 {

		/**
		 * estimate a 2D circle from given points using the circle equation
		 * x² + y² + Dx + Ey + F = 0
		 * which can be written as
		 * Dx + Ey + F = - x² - y²
		 * and solved using Ax=b -> x = inv(A)b
		 */
		template <typename List> static Circle2::CanonicalParams getParams(const List& points) {

			// number of points to use
			const int num = (int) points.size();


			// matrix with one row (equation) per point
			Eigen::Matrix<float, Eigen::Dynamic, 3> mat;
			Eigen::Matrix<float, Eigen::Dynamic, 1> res;

			// we need points.size() rows
			mat.conservativeResize(num, Eigen::NoChange);
			res.conservativeResize(num, Eigen::NoChange);

			// if the given points are far from (0,0), the result is very unstable
			// we thus subtract the average value of the given points before calculating D,E,F
			Point2<Scalar> sum(0,0);
			for (const Point2<Scalar>& p : points) { sum += p; }
			Point2<Scalar> avg = sum / num;

			// append one row (one equation) per point
			int row = 0;
			for (const Point2<Scalar>& p : points) {
				const float x = (float) p.x - avg.x;
				const float y = (float) p.y - avg.y;
				mat.row(row) << x, y, 1;
				res.row(row) << (-x*x - y*y);
				++row;
			}

			// build the inverse of A
			auto inverse = (mat.transpose() * mat).inverse() * mat.transpose();

			// calculate the output
			auto vals = inverse * res;

			// convert to ellipse parameters
			// here we need to re-add the previously subtracted average-position
			// as -D/2 is the circle's x-center we need to subtract avg.x*2 to even this out
			// same goes for y-center and F
			// is a bit tricky, but works out!
			Circle2::CanonicalParams canon( vals(0)-avg.x*2, vals(1)-avg.y*2, vals(2) + avg.x*avg.x - (vals(0)*avg.x) + avg.y*avg.y - (vals(1)*avg.y) );
			return canon;

		}

	};

}

#endif // K_CV_CIRCLEDETECTION2_H
