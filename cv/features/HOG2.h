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
	 * // http://stackoverflow.com/questions/32417531/hog-what-is-done-in-the-contrast-normalization-step
	 * // http://www.geocities.ws/talh_davidc/#cst_extract
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

	public:

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

		struct Vector : public std::vector<float> {
			void normalize() {
				float sum = 0;
				for (float  f : *this) {sum += f;}
				for (float& f : *this) {f /= sum;}
			}

			float distance(const Vector& o) const {
				float sum = 0;
				for (size_t i = 0; i < size(); ++i) {
					const float d = (*this)[i] - o[i];
					sum += d*d;
				}
				return std::sqrt(sum + 0.2);		// some sources add a constant to the sum?!
			}

		};

	private:

		/** the width/height for each block */
		const int blockSize;

		/** the width/heigh for each window */
		const int windowSize;

		/** the number of bins to use for the histogram */
		const int bins;


		/** the stride [window shift in px] that will be used during detection phase */
		const int stride = 1;


		/** histogram for each block */
		DataMatrix<Vector> blocks;

		/** histogram for each window [multiple blocks] */
		DataMatrix<Vector> windows;


	public:

		/** ctor */
		HOG2(const ImageChannel& img, const int blockSize = 8, const int bins = 9) :
			blockSize(blockSize), windowSize(blockSize*2), bins(bins) {

			// TODO: window-size must be multiple of block size
			// TODO: searching stride? (currently 1px, but requires many [unnecessary] calculations)

			precalc(img);

		}

		/** get the histogram for the block around (x,y) */
		const Vector& getBlock(const int x, const int y) const {
			if (x % stride != 0) {throw Exception("x-coordinate must be a multiple of the stride-size");}
			if (y % stride != 0) {throw Exception("y-coordinate must be a multiple of the stride-size");}
			if ((x < blockSize / 2) || (y < blockSize / 2)) {throw Exception("block position out of bounds");}
			return blocks.getConstRef(x/stride, y/stride);
		}

		/** get the historgram for the window around (x,y) */
		const Vector& getWindow(const int x, const int y) const {
			if (x % stride != 0) {throw Exception("x-coordinate must be a multiple of the stride-size");}
			if (y % stride != 0) {throw Exception("y-coordinate must be a multiple of the stride-size");}
			if ((x < windowSize / 2) || (y < windowSize / 2)) {throw Exception("window position out of bounds");}
			return windows.getConstRef(x/stride, y/stride);
		}

		/** get a feature-vector for the given location (x,y) = center and size(w,h) */
		Vector getFeature(const int x, const int y, const int w, const int h) const {

			// sanity checks
			if (x % stride != 0) {throw Exception("x-coordinate must be a multiple of the stride-size");}
			if (y % stride != 0) {throw Exception("y-coordinate must be a multiple of the stride-size");}

			if (w % blockSize != 0) {throw Exception("w must be a multiple of the block-size");}
			if (h % blockSize != 0) {throw Exception("h must be a multiple of the block-size");}

			if (windowSize != 2*blockSize) {throw Exception("not yet supported!");}

			// upper left coordinate for the area-of-interest
			const int sx = x - w/2;
			const int sy = y - h/2;

			// first window's center
			const int cx = sx + windowSize / 2;
			const int cy = sy + windowSize / 2;

			// number of windows to add to the featre-vector
			const int wx = w / blockSize - 1;		// TODO: -1 works only for blockSize = windowSize * 2
			const int wy = h / blockSize - 1;

			Vector feature;
			for (int y = 0; y < wy; ++y) {
				for (int x = 0; x < wx; ++x) {
					const int x1 = cx + x*blockSize;
					const int y1 = cy + y*blockSize;
					//std::cout << x1 << ":" << y1 << std::endl;
					const Vector& win = getWindow(x1, y1);
					_assertEqual(bins*4, win.size(), "invalid number of values in window detected");
					feature.insert(feature.end(), win.begin(), win.end());
				}
			}

			return feature;

		}


	public:

		// FOR TESTING

		/** number of degrees, each bin is wide */
		float degPerBin() const {
			return 180.0f / (float)bins;
		}

		/** convert from degress to bin number [float!] */
		float degToBin(const float deg) const {

			// sanity check
			if (deg < 0)	{throw Exception("degrees out of bounds");}
			if (deg > 360)	{throw Exception("degrees out of bounds");}

			return deg / degPerBin();

		}



		/** convert orientation + magnitude to a bin-contribution */
		Contributions getContribution(const float deg, const float mag) const {
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

		inline float atan360(const float dy, const float dx) const {
			const float rad = std::atan2(dy, dx);
			return (rad >= 0) ? (rad) : (2*M_PI+rad);
		}

		/** perform one-time calculations for fast lookups */
		void precalc(const ImageChannel& img) {
			buildBlocks(img);
			buildWindows(img);
		}

		/**
		 * step1)
		 * calculate HOG blocks [usually 8x8] around each "pixel" of the input image
		 * TODO: do not calculate for each pixel [++i] but for [i+=stride]
		 * that will be used during the matching phase. this is less accurate but faster
		 */
		void buildBlocks(const ImageChannel& img) {

			const int w = img.getWidth();
			const int h = img.getHeight();

			// number of blocks to calculate
			const int nx = img.getWidth() / stride;
			const int ny = img.getHeight() / stride;

			// get derivative images (x and y)
			const K::ImageChannel imgX = Derivative::getXcen(img);		// [-1: 0: +1]
			const K::ImageChannel imgY = Derivative::getYcen(img);		// [-1: 0: +1]

			// buffer containing HOG-Block-Histogram for every stride-th-pixel within the image
			blocks = DataMatrix<Vector>(nx, ny);

			// list of all pixels that belong to a HOG-window (centered at 0,0)
			const std::vector<ImagePoint> region = getBlockPoints(Pattern::RECTANGULAR);

			// build HOG-Histogram for each block centered at (x,y) with stride-th increment
			const int bs2 = blockSize/2;
			for (int y = bs2; y <= h-bs2; y += stride) {
				for (int x = bs2; x <= w-bs2; x += stride) {


					std::vector<HOGGradient> gradients = getGradients(imgX, imgY, x,y, region);
					const Vector hist = getHistogram(gradients);
					blocks.set(x/stride, y/stride, hist);

				}
			}

		}

		/**
		 * step2)
		 * calculate HOG windows [usually 16x16 around each "pixel" of the input image
		 */
		void buildWindows(const ImageChannel& img) {

			if (windowSize != 2*blockSize) {throw Exception("not yet supported!");}

			const int w = img.getWidth();
			const int h = img.getHeight();

			// number of windows to calculate
			const int nx = img.getWidth() / stride;
			const int ny = img.getHeight() / stride;

			// buffer containing HOG-Window-Vector for every stride-th-pixel within the image
			windows = DataMatrix<Vector>(nx, ny);

			const int bs2 = blockSize/2;
			const int ws2 = windowSize/2;

			// build combined/normalized Histogram for each Window centered at (x,y)
			for (int y = ws2; y <= h-ws2; y += stride) {
				for (int x = ws2; x <= w-ws2; x += stride) {

					// get all blocks that belong to the window
					const Vector& v00 = getBlock(x-bs2, y-bs2);	// upper left
					const Vector& v10 = getBlock(x+bs2, y-bs2);	// upper right
					const Vector& v01 = getBlock(x-bs2, y+bs2); // lower left
					const Vector& v11 = getBlock(x-bs2, y+bs2); // lower right

					// build the window
					Vector window;
					window.insert(window.end(), v00.begin(), v00.end());
					window.insert(window.end(), v10.begin(), v10.end());
					window.insert(window.end(), v01.begin(), v01.end());
					window.insert(window.end(), v11.begin(), v11.end());
					window.normalize();

					// store
					windows.set(x/stride, y/stride, window);

				}
			}

		}

//		void normalize(std::vector<HOGGradient>& gradients) {

//			float sum = 0;
//			for (const HOGGradient& hg : gradients) {
//				sum += hg.magnitude;
//			}

//			if (sum != 0) {
//				for (HOGGradient& hg : gradients) {
//					hg.magnitude /= sum;
//				}
//			}

//		}

		/** convert gradients to histogram */
		Vector getHistogram(const std::vector<HOGGradient>& gradients) {

			Vector res;
			res.resize(bins);


			for (const HOGGradient& hg : gradients) {
				const float deg = hg.direction * 180.0f / (float)M_PI;
				const Contributions c = getContribution(deg, hg.magnitude);
				if (1 == 1) {
					res[c.c1.bin] += c.c1.weight;	// split contribution
					res[c.c2.bin] += c.c2.weight;
				} else {
					res[c.c1.bin] += c.c1.weight;	// both to the same bin
					res[c.c1.bin] += c.c2.weight;	// both to the same bin
				}
			}

			return res;

//			for (const HOGGradient& hg : gradients) {
//				hist.addInterpolate(hg.direction, hg.magnitude);
//			}

//			// get
//			return hist.getVector();

		}



		/** get all individual gradients at the given location */
		std::vector<HOGGradient> getGradients(const K::ImageChannel& imgX, const K::ImageChannel& imgY, const int x, const int y, const std::vector<ImagePoint>& region) const {

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

		enum Pattern {
			RECTANGULAR,
			CIRCULAR,
		};

		/** a list of all pixels within a unit-block centered at 0,0 */
		std::vector<ImagePoint> getBlockPoints(const Pattern p) const {

			std::vector<ImagePoint> region;

			ImagePoint dst(0,0);
			const ImagePoint center(0,0);
			const int size = blockSize/2;

			// process a square region...
			for (dst.x = -size; dst.x < +size; ++dst.x) {
				for (dst.y = -size; dst.y < +size; ++dst.y) {

					if (p == RECTANGULAR) {
						region.push_back(dst);
					} else if (p == CIRCULAR) {
						// ...but use only points within a radius around the center
						const float d = center.distance(dst);
						if (d <= size) {region.push_back(dst);}
					}

				}
			}

			return region;

		}


	};

}

#endif // HOG2_H
