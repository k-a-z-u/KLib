#ifndef ELLIPSEESTIMATORPIXELRANSAC_H
#define ELLIPSEESTIMATORPIXELRANSAC_H

#include "EllipseEstimatorMath.h"
#include "EllipseEstimatorPixel.h"

#include "../../math/random/RandomIterator.h"

namespace K {

	namespace EllipseEstimator {

		/**
		 * estimate ellipse parameters by using a RANSAC approach on
		 * a given set of points, matching against white pixels within
		 * an image
		 */
		class RANSACPixel {

		public:

			struct MatchStats : public ImageMatchStats {

			};

			const float IGNORE = -1;

		private:

			int numRuns = 64;				// number of iterations
			int numSamples = 6+4;			// 6 suffice but using some more is a little more stable? espicially for thicker boarders!
			float minCoverage = 0.5f;		// at least 50% of the ellipse's outline must be covered by inliers
			float threshold = 0.5f;			// pixels >= 0.5f are accepted

			float minSize = IGNORE;
			float maxSize = IGNORE;

			float minRatio = IGNORE;
			float maxRatio = IGNORE;

		public:

			std::function<void(const Ellipse::GeometricParams&)> callback = nullptr;

		public:



			/** the iterator state */
			template <typename Element> struct Iterator {

				/** the current position within "indicies" */
				int pos;

				/** the vector with the user-data to randomly iterate */
				const std::vector<Element>& vec;

				/** the vector containing the random indices */
				const std::vector<int>& indices;

				/** ctor */
				Iterator(const int pos, const std::vector<Element>& vec, const std::vector<int>& indices) : pos(pos), vec(vec), indices(indices) {;}

				/** end-of-iteration? */
				bool operator != (const Iterator& o) const {return pos != o.pos;}

				/** next value */
				Iterator& operator ++ () {++pos; return *this;}

				/** get the user-data */
				Element operator * () {return vec[indices[pos]];}

			};



			template <typename Element> class MySegIter {

			private:

				const std::vector<Element>& vec;
				std::vector<int> indices;
				int cnt;
				std::minstd_rand gen;

			public:

				MySegIter(const std::vector<Element>& vec, const int numSamples) : vec(vec), cnt(numSamples) {
					;
				}

				void randomize() {

					const int minSize = std::min((size_t)50, vec.size());
					const int maxSize = std::min((size_t)200, vec.size());

					// draw a random segment size
					std::uniform_int_distribution<int> dSize(minSize, maxSize);
					const int size = dSize(gen);

					// draw a random segment start for a segment of the given size
					const int minStart = 0;
					const int maxStart = vec.size() - size;
					std::uniform_int_distribution<int> dStart(minStart, maxStart);
					const int start = dStart(gen);

					indices.clear();
					for (int i = 0; i < cnt; ++i) {

						// random within segment
						//int idx = start + (rand() % size);

						// linear within segment
						int idx = start + (size) * i / (cnt-1);
						if (idx < 0) {
							throw "err";
						}

						// append
						indices.push_back(idx);

					}

				}

				/** number of available random entries */
				size_t size() const {return cnt;}

				/** for-each access */
				Iterator<Element> begin() const	{ return Iterator<Element>(0, vec, indices); }

				/** for-each access */
				Iterator<Element> end() const	{ return Iterator<Element>(cnt, vec, indices); }

			};


			template <typename Element> class MyIter {

			private:

				const std::vector<Element>& vec;
				std::vector<int> indices;
				int cnt;

			public:

				MyIter(const std::vector<Element>& vec, const int numSamples) : vec(vec), cnt(numSamples) {
					for (int i = 0; i < numSamples; ++i) {
						const int idx = (vec.size()-1) * i / (numSamples-1);
						indices.push_back(idx);
					}
					int i = 0; (void) i;
				}

				/** number of available random entries */
				size_t size() const {return cnt;}

				/** for-each access */
				Iterator<Element> begin() const	{ return Iterator<Element>(0, vec, indices); }

				/** for-each access */
				Iterator<Element> end() const	{ return Iterator<Element>(cnt, vec, indices); }

			};



			/** set the number of runs to perform */
			void setNumRuns(const int numRuns) {this->numRuns = numRuns;}

			/** set the percentage for the ellipse's outline that must be covered by inliers for a result to be accepted */
			void setMinCoverage(const float coverage) {this->minCoverage = coverage;}

			/** configure a size constraint */
			void setSizeConstraint(const float minSize, const float maxSize) {this->minSize = minSize, this->maxSize = maxSize;}

			/** confgiure a ratio constraint */
			void setRatioConstraint(const float minRatio, const float maxRatio) {this->minRatio = minRatio, this->maxRatio = maxRatio;}

			/** set the pixel-brightness threshold for accepting "white" pixels as part of the ellipse */
			void setThreshold(const float threshold) {this->threshold = threshold;}

			/** set the number of samples to use for ellipse-SVD-estimation */
			void setNumSamples(const int numSamples) {this->numSamples = numSamples;}

			/** get an ellipse estimation */
			template <typename Scalar> Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& rndPoints, const K::ImageChannel& img, ImageMatchStats& _bestStats) {

				ImageMatchStats bestStats;
				Estimation bestParams;

				// provides random samples
				RandomIterator<Point2<Scalar>> it(rndPoints, numSamples);
//				MySegIter<Point2<Scalar>> it(rndPoints, numSamples);
//				std::cout << "TODO: REMOVE num runs" << std::endl;
//				size_t numRuns = rndPoints.size() / 5;		// 20%

				//MyIter<Point2<Scalar>> it(rndPoints, numSamples);
				//std::cout << "SWITCH ITER!" << std::endl;



				// process X RANSAC runs
				for (size_t i = 0; i < numRuns; ++i) {

					// estimate params from a random sample-set
					it.randomize();
					const Estimation params = Estimation::getParams<float>(it);

					// get geometric representation (if possible)
					Ellipse::CanonicalParams canon = params.toEllipse();
					canon.fixF();
					if (canon.F <= 0) {std::cout << "fix negative F" << std::endl; continue;}

					const Ellipse::GeometricParams geo = canon.toGeometric();

					// skip some erroneous values
					if (geo.a != geo.a) {continue;}
					if (geo.b != geo.b) {continue;}

					// debug output?
					if (callback) {callback(geo);}

					// enforce ellipse aspect ratio?
					if (minRatio != IGNORE && geo.getRatio() < minRatio) {continue;}
					if (maxRatio != IGNORE && geo.getRatio() > maxRatio) {continue;}

					// enforce size constraint?
					const float size = geo.a + geo.b;
					if (minSize != IGNORE && size < minSize) {continue;}
					if (maxSize != IGNORE && size > maxSize) {continue;}

					// get match stats
					const ImageMatchStats stats = Pixel::getImageStats(geo, img, threshold);

					// coverage constraint met?
					if (minCoverage != IGNORE && stats.outlineCoverage < minCoverage) {continue;}

					// have we found a better sample-set that is valid?
					//if (stats.outlineCoverage < bestStats.outlineCoverage) {continue;}
					if (stats.matchValue < bestStats.matchValue) {continue;}

					//bestVal = stats.numInliers;
					bestParams = params;
					bestStats = stats;


				}

				// done
				_bestStats = bestStats;
				return bestParams.toEllipse();

			}

		};


	}

}

#endif // ELLIPSEESTIMATORPIXELRANSAC_H
