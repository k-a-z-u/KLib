#ifndef K_CV_ELLIPSEDETECTION_H
#define K_CV_ELLIPSEDETECTION_H

#include "../../geo/Ellipse.h"

#include <eigen3/Eigen/Dense>

namespace K {

	/** estimate ellipse from a point-set */
	struct EllipseEstimator {

	public:

		/**
		 * estimate the ellipse for the given point-set using SVD.
		 * a point belongs to an ellipse identified by A,B,C,D,E,F if:
		 * Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
		 * for this point. thus we add one of those equations per given point.
		 * the SVD estimates those A,B,C,D,E,F with the lowest overall error among all points.
		 *
		 * after the estimation, errorScore contains the resulting error code
		 */
		template <typename Scalar> static Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& points, float& errorScore) {

			// matrix with one row (equation) per point
			Eigen::Matrix<float, Eigen::Dynamic, 6> mat;

			// we need points.size() rows
			mat.conservativeResize(points.size(), Eigen::NoChange);

			// append one row (one equation) per point
			for (int i = 0; i < (int) points.size(); ++i) {
				const float x = (float) points[i].x;
				const float y = (float) points[i].y;
				mat.row(i) << x*x,	x*y,	y*y,	x,		y,		1;			// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
			}

			// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
			const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
			const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
			const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

			// the 6 canonical parameters [A-F] are given by the vector in V corresponding to the smallest singular value
			// the smallest singular value belongs to the last index in U: thus "5"
			const Eigen::Matrix<float,6,1> vec = V.col(5);

			// construct cannonical parameters
			const Ellipse::CanonicalParams cParams(vec(0), vec(1), vec(2), vec(3), vec(4), vec(5));

			// done
			errorScore = D(5);
			return cParams;

		}


	};

}

#endif // K_CV_ELLIPSEDETECTION_H
