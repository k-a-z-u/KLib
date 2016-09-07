#ifndef K_CV_PLANEDETECTION3_H
#define K_CV_PLANEDETECTION3_H

#include <eigen3/Eigen/Dense>

#include "../../geo/Point3.h"
#include "../../geo/Plane3.h"
#include "../../math/EigenHelper.h"

#include "../../math/random/RandomIterator.h"


namespace K {

	template <typename Scalar> class PlaneEstimator3 {

	public:

		/** estimate plane parameters using ransac */
		class RANSAC {

		private:

			int numRuns = 64;				// number of iterations
			int numSamples = 3+2;			// 3 suffice but using some more is a little more stable?
			float minMatchRate = 0.50;		// at least 50% of the given points must reside on the ellipse;
			float maxDistance = 1.75f;		// maximum distance for a point from the plane to count as "inlier"

		public:

			/** set the number of runs to perform */
			void setNumRuns(const int numRuns) {this->numRuns = numRuns;}

			/** set the minimum number of inliers [0.0:1.0] needed for a plane to be accepted */
			void setMinMatchRate(const float rate) {this->minMatchRate = rate;}

			/** set the maximum distance for a point from the plane to count as inlier */
			void setMaxDistance(const float dist) {this->maxDistance = dist;}

			/** get the maximum distance for a point from the plane to count as inlier */
			float getMaxDistance() const {return this->maxDistance;}


			Plane3<float>::CoordinateParams get(const std::vector<Point3<Scalar>>& points) {

				// helper to randomly sample from "points"
				RandomIterator<Point3<Scalar>> it(points, numSamples);

				// number of needed inliers to accept an estimation
				const int neededInliers = (int) (minMatchRate * (float) points.size());

				Plane3<float>::CoordinateParams bestParams;
				int bestInliers = 0;

				// perform X runs
				for (int i = 0; i < numRuns; ++i) {

					// new random sample set
					it.randomize();

					// estimate parameters
					const Plane3<float>::CoordinateParams params = getParams(it);

					// number of inliers
					int numInliers = 0;
					for (const Point3<Scalar> p : points) {
						const float dst = params.getDistance(p);
						if (dst <= maxDistance) {++numInliers;}
					}

					// valid estimation?
					if (numInliers > bestInliers && numInliers >= neededInliers) {
						bestInliers = numInliers;
						bestParams = params;
					}

				}

				// done
				return bestParams;

			}


		};

		/**
		 * solve ax+by+cz+d=0 using SVD
		 * (a,b,c) denotes the plane's normal
		 */
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
				const Scalar x = p.x;
				const Scalar y = p.y;
				const Scalar z = p.z;
				mat.row(row) << x, y, z, -1;
				++row;
			}

			// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
			const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
			const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
			const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

			// vector for the smallest (4th) singular value
			const auto vec = V.col(3);

			// plane's normal and offset
			Point3f normal( (Scalar) vec(0), (Scalar) vec(1), (Scalar) vec(2));
			Scalar distance = (Scalar) vec(3);

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
