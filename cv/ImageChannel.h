#ifndef K_CV_IMAGECHANNEL
#define K_CV_IMAGECHANNEL


#include "DataMatrix.h"

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

	};

}

#endif //K_CV_IMAGECHANNEL
