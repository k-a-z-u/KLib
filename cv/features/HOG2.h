#ifndef HOG2_H
#define HOG2_H



#include "../../Assertions.h"
#include "../ImageChannel.h"
#include "../ImagePoint.h"
#include "../Derivative.h"

#include "../../math/statistics/Histogram.h"

#include <omp.h>

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
	class HOG2 {

	private:

		struct HOGGradient {
			float magnitude;
			float direction;		// in radians [0:2pi] 0 = left, pi/2 = up
		};

	private:

		/** the size of the region to examine */
		const int size;

		/** the number of bins to use for the histogram */
		const int bins;

		DataMatrix<std::vector<float>> histograms;


	public:

		/** ctor */
		HOG2(const ImageChannel& img, const int size = 4, const int bins = 8) : size(size), bins(bins) {
			precalc(img);
		}

		std::vector<float> getHistogram(const int x, const int y) const {
			return histograms.get(x,y);
		}

		float degPerBin() const {
			return 180.0f / (float)bins;
		}

		float degToBin(const float deg) const {

			if (deg < 0) {throw "error";}
			if (deg > 360) {throw "error";}

			return deg / degPerBin();

		}

		struct Contribution {
			int bin;
			float weight;
			Contribution() : bin(0), weight(0) {;}
			Contribution(int bin, float weight) : bin(bin), weight(weight) {;}
		};

		struct Contributions {
			Contribution c1;
			Contribution c2;
			Contributions(const Contribution c1, const Contribution c2) : c1(c1), c2(c2) {;}
		};

		/** convert orientation + magnitude to a bin-contribution */
		Contributions getContribution(const float deg, const float mag) {
			const float bin = degToBin(deg);
			Contribution c1, c2;
			c1.bin = std::floor(bin);
			c2.bin = std::ceil(bin);
			const float alpha = (bin - c1.bin);
			c1.bin %= bins;
			c2.bin %= bins;
			c1.weight = mag * (1.0f - alpha);
			c2.weight = mag * (alpha);
			return Contributions(c1,c2);
		}

	private:

		inline float atan360(float dy, float dx) {
			const float rad = std::atan2(dy, dx);
			return (rad >= 0) ? (rad) : (2*M_PI+rad);
		}

//		/**
//		 * 0 degree -> to the left
//		 * 90 degree -> upwards
//		 * 270 degree -> same as 90 -> upwards
//		 */
//		inline float atan180(const float y, const float x) const {
//			const float a = std::fmod(std::atan2(y, x) + 2*M_PI, 2*M_PI);
//			return (a < M_PI) ? (a) : (a - M_PI);
//		}

		void precalc(const ImageChannel& img) {

			const int nx = img.getWidth();//std::ceil((float)img.width() / (float)size);
			const int ny = img.getHeight();//std::ceil((float)img.height() / (float)size);

			// derivative images
			const K::ImageChannel imgX = Derivative::getXcen(img);		// [-1: 0: +1]
			const K::ImageChannel imgY = Derivative::getYcen(img);		// [-1: 0: +1]

			// buffer containing HOG-histogram for every pixel within the image
			histograms = DataMatrix<std::vector<float>>(nx, ny);

			// list of all pixels that belong to a HOG-window
			const std::vector<ImagePoint> region = constructRegion();

			// build HOG-Histogram for each pixel
			for (int y = 0; y < ny; ++y) {
				for (int x = 0; x < nx; ++x) {


					std::vector<HOGGradient> gradients = getGradients(imgX, imgY, x,y, region);
					normalize(gradients);
					const std::vector<float> hist = getHistogram(gradients);

					histograms.set(x,y,hist);

				}
			}

		}

		void normalize(std::vector<HOGGradient>& gradients) {

			float sum = 0;
			for (const HOGGradient& hg : gradients) {
				sum += hg.magnitude;
			}

			if (sum != 0) {
				for (HOGGradient& hg : gradients) {
					hg.magnitude /= sum;
				}
			}

		}

		/** convert gradients to histogram */
		std::vector<float> getHistogram(const std::vector<HOGGradient>& gradients) {

			std::vector<float> res;
			res.resize(bins);


			for (const HOGGradient& hg : gradients) {
				const Contributions c = getContribution(hg.direction*180.0f/M_PI, hg.magnitude);
				res[c.c1.bin] += c.c1.weight;
				res[c.c2.bin] += c.c2.weight;
			}

			return res;

//			for (const HOGGradient& hg : gradients) {
//				hist.addInterpolate(hg.direction, hg.magnitude);
//			}

//			// get
//			return hist.getVector();

		}



		/** get all individual gradients at the given location */
		std::vector<HOGGradient> getGradients(const K::ImageChannel& imgX, const K::ImageChannel& imgY, const int x, const int y, const std::vector<ImagePoint>& region) {

			std::vector<HOGGradient> gradients;

			for (size_t i = 0; i < region.size(); ++i) {

				const ImagePoint p = region[i];

				// point within the image
				const int x1 = x+p.x;
				const int y1 = y+p.y;

				// clamp
				if (x1 < 0 || x1 >= imgX.getWidth()) {continue;}
				if (y1 < 0 || y1 >= imgX.getHeight()) {continue;}

				// calculate the centered derivatives
				const auto dx = imgX.get(x1, y1);	// gradient's magnitude in x direction
				const auto dy = imgY.get(x1, y1);	// gradient's magnitude in y direction

				// calculate magnitude and direction of the gradient
				HOGGradient grad;
				grad.magnitude = std::sqrt( (dx*dx) + (dy*dy) );		// gradient's overall magnitude
				grad.direction = atan360(dy, dx);						// the gradient's direction

				gradients.push_back(grad);

			}

			return gradients;

		}

		/** construct a list of all pixels within the region */
		std::vector<ImagePoint> constructRegion() {

			std::vector<ImagePoint> region;

			ImagePoint dst(0,0);
			const ImagePoint center(0,0);

			// process a square region...
			for (dst.x = -size; dst.x < +size; ++dst.x) {
				for (dst.y = -size; dst.y < +size; ++dst.y) {

					// ...but use only points within a radius around the center
					const float d = center.distance(dst);
					//if (d <= size) {
						region.push_back(dst);
					//}

				}
			}

			return region;

		}


	};

}

#endif // HOG2_H
