#ifndef ELLIPSEESTIMATORPIXEL_H
#define ELLIPSEESTIMATORPIXEL_H

#include "../../geo/Ellipse.h"
#include "../ImageChannel.h"

#include "EllipseEstimatorMath.h"


namespace K {

	namespace EllipseEstimator {


		struct ImageMatchStats {

			/** how many percent of the ellipse's outline are covererd by inliers? */
			float outlineCoverage = 0.0f;

			/** how well do the image points fit the estimated ellipse? */
			float matchValue = 0.0f;

		};


		class Pixel {

			/** minimum pixel-value [brightness] to accept a pixel as part of the ellipse */
			float threshold = 0.5f;

			size_t numSamples = 6+6;

		public:

			/** set the threshold when to accept pixels as part of the ellipse */
			void setThreshold(const float threshold) {this->threshold = threshold;}

			/** match-quality indicator */
			struct MatchStats : public ImageMatchStats {};

			/** one matching result */
			struct Result {

				/** matched ellipse */
				Ellipse::CanonicalParams ellipse;

				/** matching quality */
				ImageMatchStats match;

			};

			/** perform matching based on ALL of the given points */
			template <typename Element> Result get(const std::vector<Element>& points, const K::ImageChannel& img) {

				Estimation est;

				if (points.size() == numSamples) {

					//est = Estimation::getParams<float>(points);
					std::cout << "TESTING" << std::endl;
					est = Estimation::getParamsEV(points);

				} else {

					std::vector<Element> samplePoints;
					for (size_t i = 0; i < numSamples; ++i) {
						const size_t idx = i * (points.size() - 1) / (numSamples - 1);
						samplePoints.push_back(points[idx]);
					}
					//est = Estimation::getParams<float>(samplePoints);
					std::cout << "TESTING" << std::endl;
					est = Estimation::getParamsEV(samplePoints);

				}

				Ellipse::CanonicalParams canon = est.toEllipse(); canon.fixF();
				const Ellipse::GeometricParams geo = canon.toGeometric();

				Result res;
				res.ellipse = canon;
				res.match = getImageStats(geo, img, threshold);
				return res;

			}


		public:

			/** get the number of white pixels within the image that are part of the ellipse */
			static ImageMatchStats getImageStats(const Ellipse::GeometricParams& geo, const K::ImageChannel& img, const float threshold) {

				ImageMatchStats stats;

				// ~360 steps around the ellipse
				const float max = 2 * (float) M_PI;
				const int steps = 360;

				//for (float rad = 0; rad < max; rad += stepSize) {
				for (int step = 0; step < steps; ++step) {

					// current position on the ellipse (in radians)
					const float rad = max * (float)step / (float)steps;

					// get the corresponding pixel within the image
					const Point2f pt = geo.getPointFor(rad);

					// is the pixel part of the image? if not, skip it
					const int x = (int) std::round(pt.x);
					const int y = (int) std::round(pt.y);
					if (!img.contains(x, y)) {continue;}

					// get the pixel's value
					const float val = img.get(x, y);

//					if (val > 0.5f) {
//						stats.numInliers++;
//						stats.outlineCoverage++;
//					}

					//stats.outlineCoverage += val;
					if (val >= threshold) {
						++stats.outlineCoverage;
						stats.matchValue += val;		// the brighter the pixel, the better the match
					}

				}

				// convert outline-coverage into [0.0:1.0]
				stats.outlineCoverage /= (float) steps;

				// done
				return stats;

			}

		};

	}

}

#endif // ELLIPSEESTIMATORPIXEL_H
