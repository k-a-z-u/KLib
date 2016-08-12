#ifndef K_CV_ELLIPSEDETECTION_H
#define K_CV_ELLIPSEDETECTION_H

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

			// scaling (used for numerical stability)
			float s;

			Estimation() : s(1) {
				params << 0,0,0,0,0,1;
			}

			Estimation(Eigen::Matrix<float, 6, 1>& params, const float s) : params(params), s(s) {;}

			/** @see getParams(); */
			Ellipse::CanonicalParams toEllipse() const {

				Eigen::Matrix<float, 6, 1> vec = params;

				// [re-adjust the scaling we added initially
				vec << vec(0), 2*vec(1), vec(2), 2*s*vec(3), 2*s*vec(4), s*s*vec(5);									// Eq 2.2

				// ensure vec is unit-length [this does NOT change the ellipse!]
				vec /= vec.norm();

				// ensure the F component is positive (otherwise negate the whole vector)
				if (vec(5) < 0) {vec = -vec;}

				// construct cannonical parameters
				return Ellipse::CanonicalParams(vec(0), vec(1), vec(2), vec(3), vec(4), vec(5));

			}

			float getErrorSampson(const float x, const float y) const {

				//const float s = 100;
				Eigen::Matrix<float, 6, 1> xi; xi << x*x, 2*x*y, y*y, 2*s*x, 2*s*y, s*s*1;

				Eigen::Matrix<float, 6, 1> theta = params;

				Eigen::Matrix<float, 6, 6> covar; covar <<
					x*x,	x*y,	0,		x,		0,		0,
					x*y,	x*x+y*y,x*y,	y,		x,		0,
					0,		x*y,	y*y,	0,		y,		0,
					x,		y,		0,		1,		0,		0,
					0,		x,		y,		0,		1,		0,
					0,		0,		0,		0,		0,		0;

				const float a = xi.dot(theta);				// same as "getError()"
				const float b = theta.dot(4*covar*theta);
				return (a*a) / b;

			}



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

			// num x num matrix
			Eigen::Matrix<float, 6, 6> squared = Eigen::Matrix<float, 6, 6>::Zero();

			// according to "ellipse fitting for computer vision, using a scaling factor of Apx. the image's size, makes everything more stable
			const float s = 100.0f;

			// append one row (one equation) per point
			for (const Point2<Scalar>& p : points) {
				const float x = (float) p.x;
				const float y = (float) p.y;
				Eigen::Matrix<float, 6, 1> xi;
				//xi << x*x,	x*y,	y*y,	x,		y,		1;			// Eq 1.1	Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
				xi << x*x,	2*x*y,	y*y,	2*s*x,	2*s*y,	1*s*s;			// Eq 2.1	Ax^2 + 2Bxy + Cy^2 + 2fDx + 2fEy + ffF = 0
				squared += xi * xi.transpose();
			}

			// normalize
			//squared = squared / (float) points.size();

			// calculate eigenvalues
			Eigen::SelfAdjointEigenSolver<decltype(squared)> solver(squared);

			// get the eigenvector for the smallest eigenvalue
			Eigen::Matrix<float, 6, 1> vec = EigenHelper::getMinEigenVector(solver.eigenvectors(), solver.eigenvalues());

			// done
			return Estimation(vec, s);




//			// number of points to use
//			const int num = std::min(64, (int)points.size());

//			// matrix with one row (equation) per point
//			Eigen::Matrix<float, Eigen::Dynamic, 6> mat;

//			// we need points.size() rows
//			mat.conservativeResize(num, Eigen::NoChange);

//			// append one row (one equation) per point
//			for (int i = 0; i < num; ++i) {
//				const int idx = i * (int)points.size() / num;
//				const Point2<Scalar>& p = points[idx] / 100.0f;
//				const float x = (float) p.x;
//				const float y = (float) p.y;
//				mat.row(i) << x*x,	x*y,	y*y,	x,		y,		1;			// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
//			}

//			// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
//			const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
//			const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
//			const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

//			// the 6 canonical parameters [A-F] are given by the vector in V corresponding to the smallest singular value
//			// the smallest singular value belongs to the last index in U: thus "5"
//			const Eigen::Matrix<float,6,1> vec = V.col(5);

//			// construct cannonical parameters
//			const Ellipse::CanonicalParams cParams(vec(0), vec(1), vec(2), vec(3), vec(4), vec(5));

//			// done
//			return cParams;

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

		/** get the number of points that have a distance-error below the given threshold */
		template <typename Scalar> static int getNumBelowThreshold(const Ellipse::GeometricParams& geo, const float distance, const std::vector<Point2<Scalar>>& points, const int stepSize) {

//			debug draw the ellipse
//			static Gnuplot gp;
//			gp << "set xrange[0:400]\n";
//			gp << "set yrange[400:0]\n";
//			GnuplotPlot plot;
//			GnuplotPlotElementLines lines; plot.add(&lines);		lines.setColorHex("#999999");
//			GnuplotPlotElementLines ellipse; plot.add(&ellipse);
//			GnuplotPlotElementPoints pts; plot.add(&pts);
//			for (float f = 0; f < M_PI*2; f+= 0.1) {
//				const Point2f pt = geo.getPointFor(f);
//				ellipse.add(GnuplotPoint2(pt.x, pt.y));
//			}
//			int xx = 0;


			//Ellipse::DistanceEstimatorBruteForce dist(geo);
			//Ellipse::DistanceEstimatorBisect dist(geo);
			EllipseDistance::AlignedSplit dist(geo, 9);			// a quality around 7 and 8 should already suffice

			//const float dist2 = distance*distance;
			int cnt = 0;


			for (int i = 0; i < (int) points.size(); i += stepSize) {

				const Point2<Scalar> p = points[i];

				//				const float curErr = params.getError((float)p.x, (float)p.y);
				//				if (std::abs(curErr) < 2) {++cnt;}
				//				if (std::abs(curErr) < threshold) {++cnt;}
				//				const float apxDistance = params.getErrorSampson((float)p.x, (float)p.y);
				//				if (std::abs(apxDistance) < dist2) {++cnt;}

//				if (++xx % 20 == 0) {
//					const Point2f n = dist.getNearest(p.x, p.y);
//					lines.addSegment( GnuplotPoint2(p.x, p.y), GnuplotPoint2(n.x, n.y) );
//					pts.add( GnuplotPoint2(p.x, p.y) );
//				}

				//const Point2f n = dist.getNearest((float)p.x, (float)p.y);
				//const float dst = n.getDistance(p);
				const float dst = dist.getDistance((float)p.x, (float)p.y);
				if (dst < distance) {++cnt;}

			}

//			gp.draw(plot);
//			gp.flush();
			//usleep(1000*100);


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

			const int numSamples = 6+4;												// 6 suffice but using some more is a little more stable? espicially for thicker boarders!
			const int numRuns = 128;
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
				if (geo.a != geo.a) {continue;}
				if (geo.b != geo.b) {continue;}

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


	};

}

#endif // K_CV_ELLIPSEDETECTION_H
