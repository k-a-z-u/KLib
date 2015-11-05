#ifndef K_CV_KERNEL_H
#define K_CV_KERNEL_H

#include <vector>
#include "../Assertions.h"
#include "DataMatrix.h"

#include <functional>

namespace K {

	class Kernel : public DataMatrix<float> {

	public:

		/** ctor */
		Kernel(const float* data, const int width, const int height) : DataMatrix(data, width, height) {
			;
		}

		/** ctor */
		Kernel(const int width, const int height) : DataMatrix(width, height) {
			;
		}


		/** get the sum of all values within the kernel */
		float getSum() const {
			float sum = 0;
			for (int i = 0; i < width*height; ++i) {
				sum += data[i];
			}
			return sum;
		}

		/** normalize the kernel (sum = 1) */
		void normalize() {
			const float sum = getSum();
			for (int i = 0; i < width*height; ++i) { data[i] /= sum; }
		}

		/** call the given function for each of the kernel's values. (x,y) are centered around (0,0) */
		void forEach(std::function<void(const int, const int, const float)> exec) const {

			// center the kernel
			const int dx = width/2;
			const int dy = height/2;

			// run function for each element
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < height; ++x) {
					exec(x-dx, y-dy, get(x,y));
				}
			}

		}

	};

}

#endif // K_CV_KERNEL_H

