#ifndef K_CV_ELLIPSEDETECTION_H
#define K_CV_ELLIPSEDETECTION_H

#include "../../geo/BBox2.h"
#include "../../geo/Ellipse.h"
#include "../../geo/EllipseDistance.h"

#include "../../math/random/RandomIterator.h"
#include "../../math/EigenHelper.h"

#include <eigen3/Eigen/Dense>

#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotPlot.h"
#include "../../misc/gnuplot/GnuplotPlotElementLines.h"
#include "../../misc/gnuplot/GnuplotPlotElementPoints.h"


namespace K {

	/** estimate ellipse from a point-set */
	struct EllipseEstimator {

	private:

		struct Estimation {

			// raw ellipse parameters A,B,C,D,E,F
			Eigen::Matrix<float, 6, 1> params;

//			// scaling (used for numerical stability)
//			float s;

			Estimation()  {
				params << 0,0,0,0,0,1;
			}

			//Estimation(const Eigen::Matrix<float, 6, 1>& params, const float s) : params(params), s(s) {;}

			static Estimation fromSVD(const Eigen::Matrix<float, 6, 1>& vec) {
				Estimation est;
				est.params = vec;
				return est;
			}

			static Estimation fromEV(const Eigen::Matrix<float, 6, 1>& vec, const float s) {
				Estimation est;
				//est.params << vec(0), 2*vec(1), vec(2), 2*s*vec(3), 2*s*vec(4), s*s*vec(5);		// book
				est.params << vec(0), vec(1), vec(2), s*vec(3), s*vec(4), s*s*vec(5);				// non-book
				est.params /= est.params.norm();
				if (est.params(5) < 0) {est.params = -est.params;}
				return est;
			}

			/** @see getParams(); */
			Ellipse::CanonicalParams toEllipse() const {

				Eigen::Matrix<float, 6, 1> vec = params;

//				// [re-adjust the scaling we added initially
//				vec << vec(0), 2*vec(1), vec(2), 2*s*vec(3), 2*s*vec(4), s*s*vec(5);									// Eq 2.2

//				// ensure vec is unit-length [this does NOT change the ellipse!]
//				vec /= vec.norm();

//				// ensure the F component is positive (otherwise negate the whole vector)
//				if (vec(5) < 0) {vec = -vec;}

				// construct cannonical parameters
				return Ellipse::CanonicalParams(vec(0), vec(1), vec(2), vec(3), vec(4), vec(5));

			}

//			float getErrorSampson(const float x, const float y) const {

//				//const float s = 100;
//				Eigen::Matrix<float, 6, 1> xi; xi << x*x, 2*x*y, y*y, 2*s*x, 2*s*y, s*s*1;

//				Eigen::Matrix<float, 6, 1> theta = params;

//				Eigen::Matrix<float, 6, 6> covar; covar <<
//					x*x,	x*y,	0,		x,		0,		0,
//					x*y,	x*x+y*y,x*y,	y,		x,		0,
//					0,		x*y,	y*y,	0,		y,		0,
//					x,		y,		0,		1,		0,		0,
//					0,		x,		y,		0,		1,		0,
//					0,		0,		0,		0,		0,		0;

//				const float a = xi.dot(theta);				// same as "getError()"
//				const float b = theta.dot(4*covar*theta);
//				return (a*a) / b;

//			}



		};

	public:

		template <typename Scalar> static Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& points) {
			Estimation est = getParams<Scalar>(points);
			return est.toEllipse();
		}


		/**
		 * estimate the ellipse for the given point-set using SVD.
		 * a point belongs to an ellipse identified by A,B,C,D,E,F if:
		 * Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
		 * for this point. thus we add one of those equations per given point.
		 * the SVD estimates those A,B,C,D,E,F with the lowest overall error among all points.
		 */
		template <typename Scalar, typename List> static Estimation getParams(const List& points) {

//			// num x num matrix
//			Eigen::Matrix<float, 6, 6> squared = Eigen::Matrix<float, 6, 6>::Zero();

//			// get a hint for the scaling value to use
//			int cnt = 0;
//			Point2f sum(0,0);
//			for (const Point2f p : points) {sum += p; ++cnt;}
//			const Point2f avg = sum / cnt;

//			// according to "ellipse fitting for computer vision, using a scaling factor of Apx. the image's size, makes everything more stable
//			const float s = (avg.x + avg.y) / 2;

//			// append one row (one equation) per point
//			for (const Point2<Scalar>& p : points) {
//				const float x = (float) p.x;
//				const float y = (float) p.y;
//				Eigen::Matrix<float, 6, 1> xi;
//				//xi << x*x,	2*x*y,	y*y,	2*s*x,	2*s*y,	1*s*s;		// Eq 2.1	book		Ax^2 + 2Bxy + Cy^2 + 2fDx + 2fEy + ffF = 0
//				xi << x*x,	x*y,	y*y,	s*x,	s*y,	1*s*s;			// Eq 2.3	non-book	Ax^2 + Bxy + Cy^2 + fDx + fEy + ffF = 0
//				squared += xi * xi.transpose();
//			}

//			// normalize
//			//squared = squared / (float) points.size();

//			// calculate eigenvalues
//			Eigen::SelfAdjointEigenSolver<decltype(squared)> solver(squared);

//			// get the eigenvector for the smallest eigenvalue
//			Eigen::Matrix<float, 6, 1> vec = EigenHelper::getMinEigenVector(solver.eigenvectors(), solver.eigenvalues());

//			// done
//			return Estimation::fromEV(vec, s);




			// number of points to use
			const int num = points.size();

			// matrix with one row (equation) per point
			Eigen::Matrix<float, Eigen::Dynamic, 6> mat;

			// we need points.size() rows
			mat.conservativeResize(num, Eigen::NoChange);

			// append one row (one equation) per point
			int row = 0;
			for (const Point2<Scalar>& p : points) {
				const float x = (float) p.x;
				const float y = (float) p.y;
				mat.row(row) << x*x,	x*y,	y*y,	x,		y,		1;			// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
				++row;
			}

			// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
			const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
			const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
			const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

			// the 6 canonical parameters [A-F] are given by the vector in V corresponding to the smallest singular value
			// the smallest singular value belongs to the last index in U: thus "5"
			const Eigen::Matrix<float,6,1> vec = V.col(5);

			return Estimation::fromSVD(vec);


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


		/**
		 * estimate ellipse parameters by using a RANSAC approach
		 */
		class RANSAC {

		public:

			struct MatchStats {

				/** how many have we found? */
				int numInliers = 0;

				/** how many percent of the ellipse's outline are covererd by inliers? */
				float outlineCoverage = 0.0f;

			};

		private:

			int stepSize = 1;				// process only every 1th input point. HANDLE WITH CARE!
			int numRuns = 64;				// number of iterations
			int numSamples = 6+4;			// 6 suffice but using some more is a little more stable? espicially for thicker boarders!
			float minCoverage = 0.50;		// at least 50% of the ellipse's outline must be covered by inliers
			float minMatchRate = 0.50;		// at least 50% of the given points must reside on the ellipse;
			float maxDistance = 1.75f;		// maximum distance for a point from the ellipse to count as "inlier"

		public:

			/** set the number of runs to perform */
			void setNumRuns(const int numRuns) {this->numRuns = numRuns;}

			/** set the minimum number of inliers [0.0:1.0] needed for an ellipse to be accepted */
			void setMinMatchRate(const float rate) {this->minMatchRate = rate;}

			/** set the percentage for the ellipse's outline that must be covered by inliers for a result to be accepted */
			void setMinCoverage(const float coverage) {this->minCoverage = coverage;}

			/** set the maximum distance for a point from the ellipse to count as inlier */
			void setMaxDistance(const float dist) {this->maxDistance = dist;}


			/** get an ellipse estimation */
			template <typename Scalar> Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& rndPoints, const std::vector<Point2<Scalar>>& allPoints, MatchStats& bestStats) {

				// number of inliers needed for a solution to be accepted
				const int minInliers = (int)(minMatchRate * (float)allPoints.size() / stepSize);

				int bestVal = 0;
				Estimation bestParams;

				// provides random samples
				RandomIterator<Point2<Scalar>> it(rndPoints, numSamples);

				// process X RANSAC runs
				for (int i = 0; i < numRuns; ++i) {

					// estimate params from a random sample-set
					it.randomize();
					const Estimation params = getParams<Scalar>(it);

					// get geometric representation (if possible)
					const Ellipse::CanonicalParams canon = params.toEllipse();
					if (canon.F <= 0) {std::cout << "fix negative F" << std::endl; continue;}

					const Ellipse::GeometricParams geo = canon.toGeometric();
					if (geo.a != geo.a) {--i; continue;}
					if (geo.b != geo.b) {--i; continue;}

					// get match stats
					const MatchStats stats = getStats(geo, maxDistance, allPoints, stepSize);

					// have we found a better sample-set that is valid?
					if ((stats.numInliers > bestVal) && (stats.numInliers >= minInliers) && (stats.outlineCoverage >= minCoverage)) {
						bestVal = stats.numInliers;
						bestParams = params;
						bestStats = stats;
					}

				}

				// done
				return bestParams.toEllipse();

			}


			/** get an ellipse estimation */
			template <typename Scalar> Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& points) {
				MatchStats best;
				return get(points, points, best);
			}



		private:


			/** get the number of points that have a distance-error below the given threshold */
			template <typename Scalar> static MatchStats getStats(const Ellipse::GeometricParams& geo, const float distance, const std::vector<Point2<Scalar>>& points, const int stepSize) {

				//static constexpr int segments = 60;
				const int outlinePixelsApx = 2.0f * (float)M_PI * std::max(geo.a, geo.b);
				const int segments = std::min(200, std::max(1, outlinePixelsApx / 4));		// number of segments [1:200]
				MatchStats stats;

				// ellipse-distance-estimator
				const EllipseDistance::AlignedSplit dist(geo, 8);			// a quality around 7 and 8 should already suffice

				// divide the ellipse into 50 segments and track, wether we got an inlier for each one
				bool degCovered[segments] = {false};

				// count the number of points within the threshold
				for (int i = 0; i < (int) points.size(); i += stepSize) {

					const Point2<Scalar> p = points[i];

					// get matching information for this point on the ellipse
					const EllipseDistance::AlignedSplit::Result res = dist.getBest(p);

					// distance between point and ellipse?
					const float dst = res.distance;

					// is this one an inlier? (distance below threshold)
					if (dst < distance) {

						++stats.numInliers;

						// which ellipse-segment is covered by this inlier?
						int deg = res.getOrigRad() * 180 / (float)M_PI;
						if (deg < 0)	{deg = 360+deg;}
						if (deg > 360)	{deg %= 360;}
						const int seg = deg * segments / 360;

						// previously uncovered segment? -> add and increase coverage
						if (!degCovered[seg]) {
							degCovered[seg] = true;
							stats.outlineCoverage += 1.0f / segments;
						}

					}

				}

				// done
				return stats;

			}

		};


		/*
		template <typename Scalar> static Ellipse::CanonicalParams getRANSAC(const std::vector<Point2<Scalar>>& points) {

			// TODO: is there something better than this?
			//std::vector<Point2<Scalar>> points = _points;
			//std::shuffle(points.begin(), points.end(), std::default_random_engine(_points.size()));

			const int numSamples = 6+4;												// 6 suffice but using some more is a little more stable? espicially for thicker boarders!
			const int numRuns = 64;
			const int stepSize = 1;													// speed-up using step-size 2
			const int minMatch = (int)(0.20f * (float)points.size() / stepSize);	// at least 25% (of the used points) must match
			const float maxDistance = 1.75f;										// maximum distance for a point from the ellipse

			int bestVal = 0;
			Estimation bestParams;

			// provides random samples
			RandomIterator<Point2<Scalar>> it(points, numSamples);

			// process X RANSAC runs
			for (int i = 0; i < numRuns; ++i) {

				// estimate params from a random sample-set
				it.randomize();
				const Estimation params = getParams<Scalar>(it);

				// get geometric representation (if possible)
				const Ellipse::GeometricParams geo = params.toEllipse().toGeometric();
				if (geo.a != geo.a) {--i; continue;}
				if (geo.b != geo.b) {--i; continue;}

				// get the number of inliers
				const int numMatch = getNumBelowThreshold(geo, maxDistance, points, stepSize);

				// have we found a better sample-set?
				if ((numMatch > bestVal) && (numMatch > minMatch)) {
					bestVal = numMatch;
					bestParams = params;
				}

			}

			// done
			return bestParams.toEllipse();

		}
		*/


	};

}

#endif // K_CV_ELLIPSEDETECTION_H
