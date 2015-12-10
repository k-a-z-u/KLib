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
	 * - no smoothing beforehand!
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
		HOG(const int size, const int bins = 8) : size(size), bins(bins) {
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
		Histogram get(const ImageChannel& src, const int x, const int y) {

			// use X bins between [0:2PI]
			Histogram h(0, 2*M_PI, bins);

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
				double dir = std::atan2(dy, dx);						// the gradient's direction

				// convert from [-pi:+po] to [0:pi]
				if (dir < 0) {dir += 2*M_PI;}
				//dir = std::fmod(dir + 2*M_PI, 2*M_PI);

				// get the bin for the given direction
				int bin = getBin(bins, dir);

				// update the histogram
				h.addToBin(bin, mag);
				//h.add(dir, mag);

			}

			// ensure histogram's sum is 1
			h.normalize();
			return h;

		}

	public:

		/** convert the given direction [0:2PI] to a bin-number */
		static int getBin(const int bins, const float dir) {
			return (int) std::round(dir * bins / (2*M_PI)) % bins;
		}

	};

}

#endif // HOG_H

