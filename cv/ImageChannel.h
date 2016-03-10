#ifndef K_CV_IMAGECHANNEL
#define K_CV_IMAGECHANNEL


#include "DataMatrix.h"
#include <functional>
#include <algorithm>

namespace K {

	class ImageChannel : public DataMatrix<float> {

	public:

		/** empty ctor */
		ImageChannel() : DataMatrix() {;}

		/** ctor with data */
		ImageChannel(const float* data, const int width, const int height) : DataMatrix(data, width, height) {;}

		/** ctor without data */
		ImageChannel(const int width, const int height) : DataMatrix(width, height) {;}

		/** set all pixels to zero */
		void zero() {
			std::fill(data.begin(), data.end(), 0);
		}

		/** set all pixels to one */
		void ones() {
			std::fill(data.begin(), data.end(), 1);
		}

		/** get the value using bi-linear interpolation */
		float getBilinear(const float x, const float y) const {
			const int x1 = (int) std::floor(x);
			const int x2 = (int) std::ceil(x);
			const float px1 = (float)x2 - x;
			const int y1 = (int) std::floor(y);
			const int y2 = (int) std::ceil(y);
			const float py1 = (float)y2 - y;
			const float vy1 = get(x1,y1) * px1 + get(x2,y1) * (1-px1);
			const float vy2 = get(x1,y2) * px1 + get(x2,y2) * (1-px1);
			return vy1 * py1 + vy2 * (1-py1);
		}

		/** call the given function for each of the channels's pixels.*/
		void forEach(std::function<void(const int, const int, const float)> exec) const {

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
