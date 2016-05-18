#ifndef K_CV_FEATURES_HOG_H
#define K_CV_FEATURES_HOG_H


#include "../../Assertions.h"
#include "../ImageChannel.h"
#include "../ImagePoint.h"
#include "../Derivative.h"

#include "../../math/statistics/Histogram.h"

#include <omp.h>

namespace K {

	const int SIZE = 12;

	struct HOGGradient {
		float magnitude;
		float direction;		// in radians [0:2pi] 0 = left, pi/2 = up
	};

	struct HOGGradients {
		float direction;		// offset (initial direction of the strongest: mag[0]
		float magnitudes[SIZE];
	};

	/**
	 * calculate the histogram-of-gradients at a given
	 * location using a provided region (size)
	 *
	 * C-HOG (circular)
	 *
	 * https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients
	 * https://www.youtube.com/watch?v=0Zib1YEE4LU
	 *
	 * - no smoothing beforehand!
	 * - [0:180] degree region!
	 *   - a 270 degree gradient is the same as a 90 degree gradient -> modulo
	 *
	 */
	class HOG {

	private:

		ImageChannel imgX;
		ImageChannel imgY;

		/** the size of the region to examine */
		const int size;

		/** the number of bins to use for the histogram */
		const int bins;

		/** pre-calculated points within the requested region */
		std::vector<ImagePoint> region;


	public:

		/** ctor */
		HOG(const ImageChannel& img, const int size = 4, const int bins = 8) : size(size), bins(bins) {
			constructRegion();
			imgX = Derivative::getXcen(img);
			imgY = Derivative::getYcen(img);
		}

	private:

		/** construct a list of all pixels within the region */
		void constructRegion() {

			ImagePoint dst(0,0);
			const ImagePoint center(0,0);

			// process a square region...
			for (dst.x = -size; dst.x <= +size; ++dst.x) {
				for (dst.y = -size; dst.y <= +size; ++dst.y) {

					// ...but use only points within a radius around the center
					const float d = center.distance(dst);
					if (d <= size) {
						region.push_back(dst);
					}

				}
			}

		}

	public:


		inline float atan360(float dy, float dx) {
			const float rad = std::atan2(dy, dx);
			return (rad >= 0) ? (rad) : (2*M_PI+rad);
		}

		int getGradients(const int x, const int y, HOGGradient* dst) {

			//HOGGradient* dst = (HOGGradient*) __builtin_assume_aligned(_dst, 32);
			int cnt = 0;

			// process each point belonging to the region (centered at (0,0))
			for (int i = 0; i < (int) region.size(); ++i) {

				// translate the region-point to the requested center (x,y)
				const ImagePoint p = region[i];
				const int x1 = x+p.x;
				const int y1 = y+p.y;


//				// ignore out-of-bounds coordinates
//				if (x1 < 1 || x1 >= src.getWidth()-1)	{continue;}
//				if (y1 < 1 || y1 >= src.getHeight()-1)	{continue;}

				// ignore out-of-bounds coordinates
				//if (x1 < 0 || x1 >= imgX.getWidth())	{continue;}
				//if (y1 < 0 || y1 >= imgX.getHeight())	{continue;}

				// calculate the centered derivatives
				const auto dx = imgX.get(x1, y1);	// gradient's magnitude in x direction
				const auto dy = imgY.get(x1, y1);	// gradient's magnitude in y direction

				// calculate magnitude and direction of the gradient
				//HOGGradient g;
				dst[cnt].magnitude = std::sqrt( (dx*dx) + (dy*dy) );		// gradient's overall magnitude
				dst[cnt].direction = atan360(dy, dx);						// the gradient's direction

				_assertNotNAN(dst[cnt].magnitude, "magnitude is NaN");
				_assertNotNAN(dst[cnt].direction, "direction is NaN");

				++cnt;

			}

			/** number of gradients */
			return cnt;

		}

		/** rotate all gradients so the strongest one points to 0 radians. returns the initial direction of the strongest */
		float relativeToBestOne(HOGGradient* gradients, const int num) const {

			auto comp = [] (const HOGGradient& a, const HOGGradient& b) {return a.magnitude < b.magnitude;};

			auto strongest = std::max_element(gradients, gradients+num, comp);
			const float strongestDir = (*strongest).direction;

			for (int i = 0; i < num; ++i) {
				gradients[i].direction -= strongestDir;							// strongest one points to 0 radians = to the left
				if (gradients[i].direction < 0) {								// ensure we stay within [0:2pi]
					gradients[i].direction = 2*M_PI + gradients[i].direction;
				}
			}

			return strongestDir;

		}

		/** normalize the given gradients. ensure the sum is 1 */
		void normalize(HOGGradient* gradients, const int num) const {
			float sum = 0;
			for (int i = 0; i < num; ++i) { sum += gradients[i].magnitude; }
			if (sum != 0) {
				for (int i = 0; i < num; ++i) {
					gradients[i].magnitude /= sum;
					_assertNotNAN(gradients[i].magnitude, "magnitude is NaN");
				}
			}
		}

		/** group the given gradients into several bins. non-symmetric version! -> [0:2pi] */
		void binify(HOGGradient* src, const int num, HOGGradients* dst, const int numBins) const {

			std::fill(dst->magnitudes, dst->magnitudes+numBins, 0);

			for (int i = 0; i < num; ++i) {
				HOGGradient& g = src[i];
				const int binIdx = std::round(g.direction * (numBins-1) / (M_PI*2));
				dst->magnitudes[binIdx] += g.magnitude;
			}

			for (int i = 0; i < numBins; ++i) {
				_assertNotNAN(dst->magnitudes[i], "magnitude is NaN");
			}

		}

//		/** get the difference (error) between the two given binified vectors */
//		static float getDistance(const std::vector<float>& h1, const std::vector<float>& h2) {
//			float sum = 0;
//			for (int i = 0; i < h1.size(); ++i) {
//				const float diff = h1[i]-h2[i];
//				sum += diff*diff;
//			}
//			return std::sqrt(sum);
//		}

		static float getDistance(const HOGGradients& h1, const HOGGradients& h2) {
			float sum = 0;
			for (int i = 0; i < SIZE; ++i) {
				const float diff = h1.magnitudes[i]-h2.magnitudes[i];
				sum += diff*diff;
			}
			return std::sqrt(sum);
		}


		DataMatrix<HOGGradients> getGradients() {
			DataMatrix<HOGGradients> res(imgX.getWidth(), imgY.getHeight());
			for (int y = size; y < imgX.getHeight()-size; ++y) {
				for (int x = size; x < imgX.getWidth()-size; ++x) {
					K::HOGGradient tmp[region.size()];// __attribute__((aligned(32)));
					HOGGradients& dst = res.get(x,y);
					const int cnt = getGradients(x, y, tmp);
					dst.direction = relativeToBestOne(tmp, cnt);
					normalize(tmp, cnt);
					binify(tmp, cnt, &dst, SIZE);
				}
			}
			return res;
		}

		/** get the HOG at the given center point within the provided image */
		template <typename InputImage>
		Histogram<float> get(const InputImage& src, const int x, const int y) {

			// use X bins between [0:2PI]
			Histogram<float> h(0, M_PI, bins);

			// process each point belonging to the region (centered at (0,0))
			for (const ImagePoint p : region) {

				// translate the region-point to the requested center (x,y)
				const int x1 = x+p.x;
				const int y1 = y+p.y;

				// ignore out-of-bounds coordinates
				if (x1 < 1 || x1 >= src.getWidth()-1)	{continue;}
				if (y1 < 1 || y1 >= src.getHeight()-1)	{continue;}

				// calculate the centered derivatives
				const float dx = Derivative::getXcen(src, x1, y1);	// gradient's magnitude in x direction
				const float dy = Derivative::getYcen(src, x1, y1);	// gradient's magnitude in y direction

				// calculate magnitude and direction of the gradient
				const float mag = std::sqrt( (dx*dx) + (dy*dy) );	// gradient's overall magnitude
				double dir = atan180(dy, dx);						// the gradient's direction


				if (dir < 0) {dir += 2*M_PI;}

				// update the histogram
				h.addInterpolate(dir, mag);

			}

			// ensure histogram's sum is 1
			h.normalize();
			return h;

		}

		/** "rotate" the histogram to ensure the largest magnitude comes first" */
		static Histogram<float> largestFirst(const Histogram<float>& h) {

			// find the index with the largest output
			float max = 0; int maxIdx = 0;
			for (int i = 0; i < h.getNumBins(); ++i) {
				if (h.get(i) > max) {
					max = h.get(i);
					maxIdx = i;
				}
			}

			// create a rotated output: bin[0] = largest output
			Histogram<float> h2(h.getMin(), h.getMax(), h.getNumBins());
			for (int i = 0; i < h.getNumBins(); ++i) {
				const int idx2 = (i+maxIdx) % h.getNumBins();
				h2.set(i, h.get(idx2));
			}

			return h2;

		}

	private:

		/**
		 * 0 degree -> to the left
		 * 90 degree -> upwards
		 * 270 degree -> same as 90 -> upwards
		 */
		double atan180(const float y, const float x) const {
			const double a = std::fmod(std::atan2(y, x) + 2*M_PI, 2*M_PI);
			return (a < M_PI) ? (a) : (a - M_PI);
		}



	};

}

#endif // K_CV_FEATURES_HOG_H

