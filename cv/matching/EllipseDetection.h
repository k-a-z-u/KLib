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

	private:

		K::EllipseEstimator::RANSACPixel ransac;
		bool combineSimilar = false;
		float blurSigma = 1.75f;

	public:

		EllipseDetection() {

			// defaults
			ransac.setMinCoverage(0.70f);
			ransac.setRatioConstraint(1.0f, 1.5f);
			ransac.setSizeConstraint(30, 150);						// ransac.setSizeConstraint(15.0f, 180.0f);
			ransac.setThreshold(0.28f);								// minimal pixel brightness for accepting
			ransac.setNumSamples(12);								// number of samples for SVD
			ransac.setNumRuns(15);									// number of RANSAC runs

		}

		K::EllipseEstimator::RANSACPixel& getRANSAC() {
			return ransac;
		}

		/** whether to combine similar ellipses into one */
		void setCombineSimilar(const bool combine) {
			this->combineSimilar = combine;
		}

		/** set the sigma to use for gaussian blur before performing ellipse estimation based on blurred pixels */
		void setBlurSigma(const float sigma) {
			this->blurSigma = sigma;
		}

		/** perform ellipse-detection on a given black/white image with 1pixel wide,white edges */
		std::vector<K::Ellipse::GeometricParams> getFromEdgeImage(const K::ImageChannel& imgEdges) {

			const std::vector<std::vector<K::Point2i>> allSegments = getSegments(imgEdges);

			// TODO
			//const std::vector<std::vector<K::Point2i>> splitSegments = getSegmentsSplitFix(allSegments, 300);
			const std::vector<std::vector<K::Point2i>> splitSegments = getSegmentsSplitVar(allSegments, 75, 450);


			const K::ImageChannel imgEdgesBlur = getBlurred(imgEdges);

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

			if (combineSimilar) {
				std::vector<K::Ellipse::GeometricParams> ellipsesDistinct = filterDuplicates(ellipses);
				return ellipsesDistinct;
			} else {
				return ellipses;
			}

		}


	private:


		K::ImageChannel getBlurred(const K::ImageChannel& imgEdges) {

			// remove short edges [isolated pixels]
			K::ImageChannel imgEdgesCleaned = K::CV::Clean::avgThreshold(imgEdges, 1, 0.33f);

			K::ImageChannel imgEdgesBlur = imgEdgesCleaned;
			//imgEdgesBlur = K::Dilate::apply(imgEdgesBlur, 2, K::Dilate::Shape::CIRCLE, 1.0f, 0.01f);

			// slightly blur the image (spread edges)
			if (blurSigma != 0) {
				K::CV::Gauss gauss(blurSigma, blurSigma);
				imgEdgesBlur = gauss.filter(imgEdgesBlur);
				imgEdgesBlur = K::CV::Normalize::run(imgEdgesBlur);
			}

			K::ImageFactory::writePNG("/tmp/bla.png", imgEdgesBlur);

			return imgEdgesBlur;

		}

		std::vector<std::vector<K::Point2i>> getSegmentsSplitFix(const std::vector<std::vector<K::Point2i>>& segments, const size_t maxSize) {

			std::vector<std::vector<K::Point2i>> splitSegments;

			// process every input segment
			for (const std::vector<K::Point2i>& seg : segments) {

				// keep small segments "as-is"
				if (seg.size() <= maxSize) {

					splitSegments.push_back(seg);

				} else {

					const int numSegments = (int) std::ceil((float)seg.size() / (float)maxSize);
					const float sizePerSegment = seg.size() / numSegments;

					// 50% overlapping segments
					for (float segNr = 0; segNr <= numSegments - 0.4; segNr += 0.5) {
						const int start = segNr * sizePerSegment;
						const int end = std::min((size_t)(start + sizePerSegment), seg.size()-1);
						const std::vector<K::Point2i> pts(seg.begin()+start, seg.begin()+end);
						splitSegments.push_back(pts);
					}

				}

			}

			return splitSegments;

		}

		/**
		 * split all large segments into several [overlapping] smaller-sized segments
		 */
		std::vector<std::vector<K::Point2i>> getSegmentsSplitVar(const std::vector<std::vector<K::Point2i>>& segments, const float minSize, const float maxSize) {

			// output
			std::vector<std::vector<K::Point2i>> splitSegments;

			// split large segments several times
			for (const std::vector<K::Point2i>& seg : segments) {

				// skip this segment?
				if (seg.size() < minSize) {continue;}

				const float stepSize = 0.4f;
				const int maxSegs = std::ceil(seg.size() / minSize);		// largest number of segment divisions to check
				const int minSegs = std::ceil(seg.size() / maxSize);		// smallest number of segment divisions to check

				for (float segs = minSegs; segs <= maxSegs; segs+=stepSize) {

					const float segSize = seg.size() / segs;

					// 50% overlapping segments
					for (float i = 0; i < segs; i += 0.50f) {
						const int start = (int)(i*segSize);
						const int end = std::min((int)(start + segSize), (int)(seg.size()-1));
						const int num = end-start;
						if (num < 20) {continue;}
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
