#ifndef ELLIPSEDETECTION_H
#define ELLIPSEDETECTION_H

#include "EllipseEstimator.h"
#include "../ImageChannel.h"
#include "../segmentation/Segmentation.h"

#include "../filter/Clean.h"
#include "../filter/Gauss.h"
#include "../filter/Normalize.h"
#include "../filter/Dilate.h"

#include "../ImageFactory.h"

#include <omp.h>

namespace K {

	class EllipseDetection {

	public:

		/** perform ellipse-detection on a given black/white image with 1pixel wide,white edges */
		std::vector<K::Ellipse::GeometricParams> getFromEdgeImage(const K::ImageChannel& imgEdges) {

			const std::vector<std::vector<K::Point2i>> allSegments = getSegments(imgEdges);
			const std::vector<std::vector<K::Point2i>> splitSegments = getSegmentsSplit(allSegments);

			const K::ImageChannel imgEdgesBlur = getBlurred(imgEdges);

			// ransac setup
			K::EllipseEstimator::RANSACPixel ransac;
			ransac.setMinCoverage(0.70f);
			ransac.setRatioConstraint(1.0f, 1.5f);
			ransac.setSizeConstraint(30, 150);						// ransac.setSizeConstraint(15.0f, 180.0f);
			ransac.setThreshold(0.28f);								// minimal pixel brightness for accepting
			ransac.setNumSamples(12);								// number of samples for SVD
			ransac.setNumRuns(15);									// number of RANSAC runs

			std::vector<K::Ellipse::GeometricParams> ellipses;

			//#pragma omp parallel for
			for (size_t i = 0; i < splitSegments.size(); ++i) {

				// current segment
				const std::vector<K::Point2i>& set = splitSegments[i];

				// perform detection for the current segment
				K::EllipseEstimator::RANSACPixel::MatchStats stats;
				K::Ellipse::CanonicalParams canon = ransac.get(set, imgEdgesBlur, stats);

				//if (canon.F < 0) {continue;}

				K::Ellipse::GeometricParams geo = canon.toGeometric();
				if (geo.a != geo.a || geo.b != geo.b) {continue;}

				ellipses.push_back(geo);

			}

//			return ellipses;

			std::vector<K::Ellipse::GeometricParams> ellipsesDistinct = filterDuplicates(ellipses);
			return ellipsesDistinct;

		}


	private:


		K::ImageChannel getBlurred(const K::ImageChannel& imgEdges) {

			// remove short edges [isolated pixels]
			K::ImageChannel imgEdgesCleaned = K::Clean::avgThreshold(imgEdges, 1, 0.25f);

			// slightly blur the image (spread edges)
			K::Gauss gauss(1.5f, 1.5f);
			K::ImageChannel imgEdgesBlur = imgEdgesCleaned;
			//imgEdgesBlur = K::Dilate::apply(imgEdgesBlur, 1, K::Dilate::Shape::CIRCLE, 1.0f, 0.01f);
			imgEdgesBlur = gauss.filter(imgEdgesBlur);
			imgEdgesBlur = K::Normalize::run(imgEdgesBlur);

			//K::ImageFactory::writePNG("/tmp/bla.png", imgEdgesBlur);

			return imgEdgesBlur;

		}


		std::vector<std::vector<K::Point2i>> getSegmentsSplit(const std::vector<std::vector<K::Point2i>>& segments) {

			const float segmentMin = 80.0f;									// min number of points per segment (smallest segment before splitting starts)
			const float segmentMax = 300.0f;								// max number of points per segment (largest segment)
			const int stepSize = 2;

			std::vector<std::vector<K::Point2i>> splitSegments;

			// split large segments several times
			for (const std::vector<K::Point2i>& seg : segments) {

				const int maxSegs = std::ceil(seg.size() / segmentMin);		// largest number of segment divisions to check
				const int minSegs = std::ceil(seg.size() / segmentMax);		// smallest number of segment divisions to check

				for (int segs = minSegs; segs <= maxSegs; segs+=stepSize) {

					const float segSize = seg.size() / segs;

					for (int i = 0; i < segs; ++i) {
						const int start = i*segSize;
						const int end = start + segSize;
						const std::vector<K::Point2i> pts(seg.begin()+start, seg.begin()+end);
						splitSegments.push_back(pts);
					}

				}

			}

//			K::ImageChannel img(640, 480);
//			for (const std::vector<K::Point2i>& seg : splitSegments) {
//				for (const K::Point2i p : seg) {
//					img.set(p.x, p.y, 1.0);
//				}
//			}
//			K::ImageFactory::writePNG("/tmp/segments.png", img);

			return splitSegments;

		}


		std::vector<std::vector<K::Point2i>> getSegments(const K::ImageChannel& imgEdges)  {

			// get all segments within the image
			std::vector<K::Segment<float>> segments = K::Segmentation::getSegments(imgEdges);
			std::vector<std::vector<K::Point2i>> result;

			for (const K::Segment<float>& seg : segments) {

				if (seg.points.size() < 16)	{continue;}							// ingore very small segments
				if (seg.avg == 0)			{continue;}							// ignore black parts

				result.push_back(seg.points);

			}

			return result;

		}

		/** combine ellipses that are similar and return a new ellipse which is given by their average */
		std::vector<K::Ellipse::GeometricParams> filterDuplicates(const std::vector<K::Ellipse::GeometricParams>& src) {

			std::vector<K::Ellipse::GeometricParams> filtered;

			for (const K::Ellipse::GeometricParams& e1 : src) {

				bool unique = true;
				for (K::Ellipse::GeometricParams& e2 : filtered) {

					const float dCenterDiff = e1.center.getDistance(e2.center);
					const float dSizeRatio = std::max(e1.getCircumfence(), e2.getCircumfence()) / std::min(e1.getCircumfence(), e2.getCircumfence());
					const bool isSimilar = (dCenterDiff < 16) && (dSizeRatio < 1.12f);

					// if this ellipse is similar to an existing one, join the two
					if (isSimilar) { unique = false; e2.mix(e1, 0.50f); break; }

				}

				if (unique) { filtered.push_back(e1); }

			}

			return filtered;

		}



	};

}

#endif // ELLIPSEDETECTION_H
