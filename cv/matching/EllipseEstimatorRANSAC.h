#ifndef ELLIPSEESTIMATORRANSAC_H
#define ELLIPSEESTIMATORRANSAC_H

#include "EllipseEstimatorMath.h"

#include "../../math/random/RandomIterator.h"

#include "../../geo/EllipseDistance.h"

namespace K {

	namespace EllipseEstimator {


		/**
		 * estimate ellipse parameters by using a RANSAC approach on
		 * a given set of points
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
					const Estimation params = Estimation::getParams<Scalar>(it);

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


	};

}

#endif // ELLIPSEESTIMATORRANSAC_H
