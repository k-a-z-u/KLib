#ifndef HOG_H
#define HOG_H

#include "../ImageChannel.h"
#include "../ImagePoint.h"
#include "../Derivative.h"

#include "../../math/statistics/Histogram.h"

namespace K {

	struct HOGGradient {
		float magnitude;
		float direction;		// in radians [0:2pi] 0 = left, pi/2 = up
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

		/** the size of the region to examine */
		const int size;

		/** the number of bins to use for the histogram */
		const int bins;

		/** pre-calculated points within the requested region */
		std::vector<ImagePoint> region;


	public:

		/** ctor */
		HOG(const int size = 4, const int bins = 8) : size(size), bins(bins) {
			constructRegion();
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
					if (d <= size) {region.push_back(dst);}

				}
			}

		}

	public:


		/** get all gradients within the block around (x,y) */
		std::vector<HOGGradient> getGradients(const ImageChannel& src, const int x, const int y) {

			std::vector<HOGGradient> vec(region.size());

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
				HOGGradient g;
				g.magnitude = std::sqrt( (dx*dx) + (dy*dy) );				// gradient's overall magnitude
				g.direction = std::atan2(dy, dx);							// the gradient's direction

				vec.push_back(g);

			}

			return vec;

		}

		/** rotate all gradients so the strongest one points to 0 radians */
		void relativeToBestOne(std::vector<HOGGradient>& gradients) {

			auto comp = [] (const HOGGradient& a, HOGGradient& b) {return a.magnitude < b.magnitude;};
			auto it = std::max_element(gradients.begin(), gradients.end(), comp);
			HOGGradient strongest = *it;

			for (HOGGradient& g : gradients) {
				g.direction -= strongest.direction;				// strongest one points to 0 radians = to the left
				if (g.direction < 0) {g.direction += M_PI*2;}	// ensure we stay within [0:2pi]
			}

		}

		/** normalize the given gradients. ensure the sum is 1 */
		void normalize(std::vector<HOGGradient>& gradients) const {
			float sum = 0;
			for (HOGGradient& g : gradients) { sum += g.magnitude; }
			for (HOGGradient& g : gradients) { g.magnitude /= sum; }
		}

		/** group the given gradients into several bins. non-symmetric version! -> [0:2pi] */
		std::vector<float> binify(std::vector<HOGGradient>& gradients, const int numBins) const {

			std::vector<float> bins(numBins);

			for (HOGGradient& g : gradients) {
				const int binIdx = g.direction * numBins / (M_PI*2);
				bins[binIdx] += g.magnitude;
			}

			return bins;

		}

		/** get the difference (error) between the two given binified vectors */
		static float getDistance(const std::vector<float>& h1, const std::vector<float>& h2) {
			float sum = 0;
			for (int i = 0; i < h1.size(); ++i) {
				const float diff = h1[i]-h2[i];
				sum += diff*diff;
			}
			return std::sqrt(sum);
		}



		/** get the HOG at the given center point within the provided image */
		Histogram<float> get(const ImageChannel& src, const int x, const int y) {

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

#endif // HOG_H

