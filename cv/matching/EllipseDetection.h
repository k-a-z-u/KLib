#ifndef K_CV_ELLIPSEDETECTION_H
#define K_CV_ELLIPSEDETECTION_H

#include "../../geo/Ellipse.h"

#include "../../math/random/RandomIterator.h"

#include <eigen3/Eigen/Dense>

namespace K {

	/** estimate ellipse from a point-set */
	struct EllipseEstimator {

	public:

		template <typename Scalar> static Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& points) {
			return get<Scalar>(points, (int) points.size());
		}

		/**
		 * estimate the ellipse for the given point-set using SVD.
		 * a point belongs to an ellipse identified by A,B,C,D,E,F if:
		 * Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
		 * for this point. thus we add one of those equations per given point.
		 * the SVD estimates those A,B,C,D,E,F with the lowest overall error among all points.
		 */
		template <typename Scalar, typename List> static Ellipse::CanonicalParams get(const List& points, const int size) {

			// number of points to use
			const int num = std::min(64, size);

			// matrix with one row (equation) per point
			Eigen::Matrix<float, Eigen::Dynamic, 6> mat;

			// we need points.size() rows
			mat.conservativeResize(num, Eigen::NoChange);

			// append one row (one equation) per point
			for (int i = 0; i < num; ++i) {
				const int idx = i * size / num;
				const Point2<Scalar>& p = points[idx];
				const float x = (float) p.x;
				const float y = (float) p.y;
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
			return cParams;

		}

//		template <typename Scalar> static float getError(const Ellipse::CanonicalParams& params, const std::vector<Point2<Scalar>>& points) {

//			float errSum = 0;
//			for (const Point2<Scalar>& p : points) {
//				const float curErr = params.getError(p.x, p.y);
//				const float err2 = params.getErrorSampson(p.x, p.y);
//				errSum += curErr * curErr;
//			}
//			return errSum / points.size();

//		}

		/** get the number of points that have an error below the given threshold */
		template <typename Scalar> static int getNumBelowThreshold(const Ellipse::CanonicalParams& params, const float threshold, const std::vector<Point2<Scalar>>& points) {

			int cnt = 0;
			for (const Point2<Scalar>& p : points) {
				const float curErr = params.getError((float)p.x, (float)p.y);
				const float err2 = params.getErrorSampson((float)p.x, (float)p.y);
				//if (std::abs(curErr) < threshold) {++cnt;}
				if (std::abs(err2) < 15000.5) {++cnt;}
			}
			return cnt;

		}

		template <typename Scalar> static Ellipse::CanonicalParams getRemoveWorst(const std::vector<Point2<Scalar>>& _points) {

			std::vector<Point2<Scalar>> points = _points;
			Ellipse::CanonicalParams params;
			const int toRemove = points.size() * 0.15;
			const int removePerRun = points.size() * 0.04;

			for (int i = 0; i < toRemove/removePerRun; ++i) {

				// estimate params from a random sample-set
				params = get<Scalar>(points);

				// sort points by error
				auto comp = [&] (const Point2<Scalar>& p1, const Point2<Scalar>& p2) { return (params.getError(p1.x,p1.y) > params.getError(p2.x,p2.y)); };
				std::sort(points.begin(), points.end(), comp);

				// remove the worst 2.5%
				points.resize(points.size() - removePerRun);


			}

			return params;

		}


		template <typename Scalar> static Ellipse::CanonicalParams getRANSAC(const std::vector<Point2<Scalar>>& _points) {

			// TODO: is there something better than this?
			std::vector<Point2<Scalar>> points = _points;
			std::shuffle(points.begin(), points.end(), std::default_random_engine(_points.size()));

			const int numSamples = 12;									// 6 would suffice but 14 is more stable
			const int numRuns = 64;
			const int minMatch = (int)(0.5f * (float)points.size());	// at least 50% must match


			int bestVal = 0;
			Ellipse::CanonicalParams bestParams(0,0,0,0,0,0);

			for (int i = 0; i < numRuns; ++i) {

				// provides random samples
				RandomIterator<Point2<Scalar>> it(points, numSamples);

				// estimate params from a random sample-set
				Ellipse::CanonicalParams params = get<Scalar>(it, numSamples);
				//params.normalize();

				// get the number of inliers
				const int numMatch = getNumBelowThreshold(params, 0.02f, points);

				// found a better sample-set?
				if (numMatch > bestVal) {//{ && numMatch > minMatch) {
					bestVal = numMatch;
					bestParams = params;
				}

			}

			return bestParams;

		}


	};

}

#endif // K_CV_ELLIPSEDETECTION_H
