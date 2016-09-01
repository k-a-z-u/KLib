#ifndef K_CV_PLANEDETECTION3_H
#define K_CV_PLANEDETECTION3_H

#include <eigen3/Eigen/Dense>

#include "../../geo/Point3.h"
#include "../../geo/Plane3.h"
#include "../../math/EigenHelper.h"

namespace K {

	template <typename Scalar> class PlaneEstimator3 {

	public:

		template <typename List> static Plane3<float>::CoordinateParams getParams(const List& points) {

			// number of points to use
			const int num = (int) points.size();

			// matrix with one row (equation) per point
			Eigen::Matrix<double, Eigen::Dynamic, 4> mat;

			// we need points.size() rows
			mat.conservativeResize(num, Eigen::NoChange);

			// append one row (one equation) per point
			int row = 0;
			for (const Point3<Scalar>& p : points) {
				const float x = (float) p.x;
				const float y = (float) p.y;
				const float z = (float) p.z;
				mat.row(row) << x, y, z, 1;
				++row;
			}

			// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
			const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
			const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
			const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();


			const auto vec = V.col(3);

			Point3f normal(vec(0), vec(1), vec(2));
			float distance = vec(3);

			// ensure the normal is normalized. this also adjusts the distance!
			const float len = normal.getLength();
			normal /= len;
			distance /= len;

			// ensure the normal points upwards [is exactly the same.. just turned upside down.. thats the degree of freedom we have anyways]
			//if (normal.z < 0) {normal = -normal;} // AND WHAT SHOULD WE DO FOR -x or -y?!....

			// done;
			return Plane3<float>::CoordinateParams(normal, distance);

		}


		/**
		 * very simple plane estimator
		 * NOTE: needs approx at-least 10 points to function properly!
		 *
		 * estimates the plane's normal and "center"-point
		 * center = average of all of the given points
		 * normal = smallest eigenvector using the point's covariance
		 */
		template <typename List> static Plane3<float>::NormalParams getNormalParams(const List& points) {

			// number of points to use
			const int num = (int) points.size();

			// sanity check
			//_assertTrue(num > 10, "needs at-least 10 points to function properly");
			if (num < 10) {std::cout << "needs at-least 10 points to function properly" << std::endl;}


			// calculate the average of all of the given points. this is the plane's center
			K::Point3<Scalar> sum(0,0,0);
			for (const Point3<Scalar>& p : points) {
				sum += p;
			}
			const K::Point3<Scalar> avg = sum / num;

			// now calculate the covariance around the center
			Eigen::Matrix<double, 3, 3> mat = Eigen::Matrix<double, 3, 3>::Zero();
			for (const Point3<Scalar>& _p : points) {
				const Point3<Scalar>p = _p - avg;
				Eigen::Matrix<double, 3, 1> vec; vec << p.x, p.y, p.z;
				mat += vec * vec.transpose();
			}
			mat /= num;

			// the smallest eigenvector of the covariance-matrix denotes the normal for the point-cloud
			Eigen::SelfAdjointEigenSolver<decltype(mat)> solver(mat);
			auto vNormal = EigenHelper::getMinEigenVector(solver.eigenvectors(), solver.eigenvalues());
			K::Point3<Scalar> normal(vNormal(0), vNormal(1), vNormal(2));

			// done
			return Plane3<float>::NormalParams(normal, avg);


		}

	};

}

#endif // K_CV_PLANEDETECTION3_H
