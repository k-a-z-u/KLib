#ifndef K_CV_H
#define K_CV_H

#include "ImageChannel.h"

/** openCV */
namespace cv {
	class Mat;
}

namespace K {

	class CV {

	public:

		static ImageChannel fromOpenCV(const cv::Mat& m) {
			ImageChannel img(m.cols, m.rows);
			for (int y = 0; y < m.rows; ++y) {
				for (int x = 0; x < m.cols; ++x) {
					const float val = m.at<uint8_t>(y,x);
					img.set(x,y,val/255.0f);
				}
			}
			return img;
		}

		static DataMatrix<uint8_t> fromOpenCV8(const cv::Mat& m) {
			DataMatrix<uint8_t> img(m.cols, m.rows);
			for (int y = 0; y < m.rows; ++y) {
				for (int x = 0; x < m.cols; ++x) {
					const uint8_t val = m.at<uint8_t>(x,y);
					img.set(x,y,val);
				}
			}
			return img;
		}

	};

}

#endif // K_CV_H
