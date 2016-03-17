#ifndef K_CV_FILTER_INTERPOLATION_H
#define K_CV_FILTER_INTERPOLATION_H

#include "../ImageChannel.h"

namespace K {

	/**
	 * provides various interpolation techniques to
	 * access image pixel data
	 */
	class Interpolation {

		using Scalar = float;

	public:


		/** do not use any interpolation. just round to the nearest pixel */
		static Scalar none(const ImageChannel& img, const float x, const float y) {
			return img.get((int)std::round(x), (int)std::round(y));
		}

		/** use bilinear interpolation (4 samples) */
		static Scalar bilinear(const ImageChannel& img, const float x, const float y) {

			const int x1 =		(int) std::floor(x);	// low
			const int x2 =		(int) std::ceil(x);		// high

			const int y1 =		(int) std::floor(y);	// low
			const int y2 =		(int) std::ceil(y);		// high

			const float px1 =	(float)x2 - x;
			const float py1 =	(float)y2 - y;

			const float vy1 = img.getClamped(x1,y1) * px1 + img.getClamped(x2,y1) * (1-px1);
			const float vy2 = img.getClamped(x1,y2) * px1 + img.getClamped(x2,y2) * (1-px1);
			return vy1 * py1 + vy2 * (1-py1);

		}

		static Scalar bicubic(const ImageChannel& img, const float x, const float y) {

			int ix = (int) std::floor(x);
			int iy = (int) std::floor(y);

			float arr[4][4] = {0};
			for (int x1 = 0; x1 <= 3; ++x1) {
				for (int y1 = 0; y1 <= 3; ++y1) {
					arr[x1][y1] = img.getClamped(ix+x1-1, iy+y1-1);
				}
			}
			return clamp(bicubicInterpolate(arr, x-(float)ix, y-(float)iy), 0, 1);



		}

		static float cubicInterpolate (const float p[4], const float x) {
			return p[1] + 0.5f * x*(p[2] - p[0] + x*(2.0f*p[0] - 5.0f*p[1] + 4.0f*p[2] - p[3] + x*(3.0f*(p[1] - p[2]) + p[3] - p[0])));
		}

		/** x and y within [0:1] */
		static float bicubicInterpolate (const float p[4][4], const float x, const float y) {
			float arr[4];
			arr[0] = cubicInterpolate(p[0], y);
			arr[1] = cubicInterpolate(p[1], y);
			arr[2] = cubicInterpolate(p[2], y);
			arr[3] = cubicInterpolate(p[3], y);
			return cubicInterpolate(arr, x);
		}

		static float clamp(const float v, const float min, const float max) {
			if (v < min) {return min;}
			if (v > max) {return max;}
			return v;
		}


	};

}

#endif // K_CV_FILTER_INTERPOLATION_H
