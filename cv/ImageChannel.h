#ifndef K_CV_IMAGECHANNEL
#define K_CV_IMAGECHANNEL


#include "DataMatrix.h"
#include <functional>
#include <algorithm>

namespace K {

	class ImageChannel : public DataMatrix<float> {

	public:

		/** ctor with data */
		ImageChannel(const float* data, const int width, const int height) : DataMatrix(data, width, height) {;}

		/** ctor without data */
		ImageChannel(const int width, const int height) : DataMatrix(width, height) {;}

		/** set all pixels to zero */
		void zero() {
			std::fill(data.begin(), data.end(), 0);
		}

		/** normalize the image = set all values between [0.0:1.0] */
		void normalize() {

			const float min = *std::min_element(data.begin(), data.end());
			const float max = *std::max_element(data.begin(), data.end());
			const float diff = max - min;

			auto update = [&] (const int x, const int y, const float val) {(void) x; (void) y; return (val - min) / diff;};
			forEachModify(update);

		}

		/** call the given function for each of the channels's pixels.*/
		void forEach(std::function<float(const int, const int, const float)> exec) const {

			// run function for each element
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					exec(x, y, get(x,y));
				}
			}

		}

		/** call the given function for each of the channels's pixels.*/
		void setEach(std::function<float(const int, const int)> func) {

			// run function for each element
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					set(x,y, func(x,y));
				}
			}

		}

		/** call the given function for each of the channels's pixels.*/
		void forEachModify(std::function<float(const int, const int, const float)> exec) {

			// run function for each element
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					const float val = exec(x, y, get(x,y));
					set(x,y,val);
				}
			}

		}

	};

}

#endif //K_CV_IMAGECHANNEL
