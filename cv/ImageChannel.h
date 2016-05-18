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


		int setIdx;
		ImageChannel& operator << (const float val) {
			data[0] = val;
			setIdx = 1;
			return *this;
		}

		ImageChannel& operator , (const float val) {
			data[setIdx] = val;
			++setIdx;
			return *this;
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

		/** clamped access (repeat edge values) to the underlying image data */
		float getClamped (const int x, const int y) const {
			return get( clamp(x,0,width-1), clamp(y,0,height-1) );
		}

	private:

		/** clamp the given value to [min:max] */
		static inline int clamp(const int val, const int min, const int max) {
			if (val < min) {return min;}
			if (val > max) {return max;}
			return val;
		}

	};

}

#endif //K_CV_IMAGECHANNEL
