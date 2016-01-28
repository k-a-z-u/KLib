#ifndef HOG_H
#define HOG_H

#include "../ImageChannel.h"
#include "../ImagePoint.h"
#include "../Derivative.h"

#include "../../math/statistics/Histogram.h"

namespace K {

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

