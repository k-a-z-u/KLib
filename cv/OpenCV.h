#ifndef K_OPENCV_H
#define K_OPENCV_H

#include <opencv2/opencv.hpp>
#include "ImageChannel.h"

namespace K {

	namespace CV {

		/** convert from openCV matrix to K::ImageChannel */
		static inline K::ImageChannel cv_to_k(const cv::Mat& mat) {

			cv::Mat f32;
			cv::Mat tmp; mat.copyTo(tmp);

			// if the input is RGB, convert to gray
			if (tmp.type() == CV_8UC3) {
				cv::cvtColor(tmp, tmp, CV_BGR2GRAY);			// RGB uint8_t to gray
			}

			// convert single-channel matrix to ImageChannel
			if (tmp.type() == CV_8U) {
				tmp.convertTo(f32, CV_32F, 1.0f/255.0f);		// uint8_t to float
			} else if (tmp.type() == CV_32F) {
				f32 = tmp;										// float to float
			}

			// memcopy
			return ImageChannel((const float*) f32.data, f32.cols, f32.rows);

		}

		static inline cv::Mat k_to_cv(const K::ImageChannel& img) {

			cv::Mat mat (img.getHeight(), img.getWidth(), CV_32F);
			memcpy(mat.data, img.getData(), img.getSizeBytes());

			mat.convertTo(mat, CV_8U, 255.0f);

			return mat;

		}

	}

}

#endif // K_OPENCV_H
