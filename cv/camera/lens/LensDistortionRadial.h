#ifndef LENSDISTORTIONRADIAL_H
#define LENSDISTORTIONRADIAL_H

#include "../../../Assertions.h"
#include "../../../geo/Point2.h"
#include "../../ImageChannel.h"
#include "../../filter/Interpolation.h"


namespace K {

	/**
	 * this class describes radial lens distortion:
	 * x_dist = x_undist * F;
	 *	F = (1 + k_1*r^2 + k_2*r^4 + k_3*r^6 + ...)
	 *	r = sqrt(x_undist^2 + y_undist^2)
	 */
	class LensDistortionRadial {

	public:

		/** get the multiplication factor for the given point */
		template <typename T> static float getFact(const Point2f& point, const T* params, const int num) {

			// sanity checks. x and y must be in range [-0.5:+0.5]
			_assertBetween(point.x, -0.5, +0.5, "x-coordinate out of range");
			_assertBetween(point.y, -0.5, +0.5, "y-coordinate out of range");
			_assertBetween(num, 1, 6, "number of coefficients out of range");

			// distance from the center (0,0)
			const float radius = point.getLength();

			// calculate
			float res = 1;
			for (int i = 0; i < num; ++i) {
				res += params[i] * std::pow(radius, 2 + i*2);
			}

			// done
			return res;

		}


		/** undistort the given point using the provided k-factors */
		template <typename T> static Point2f undistort(const Point2f& distorted, const T* params, const int num) {
			const float fact = getFact(distorted, params, num);
			return distorted / fact;
		}

		/** distort the given point using the provided k-factors */
		template <typename T> static Point2f distort(const Point2f& undistorted, const T* params, const int num) {
			const float fact = getFact(undistorted, params, num);
			return undistorted * fact;
		}


		template <typename T> static ImageChannel undistort(const ImageChannel& src, const T* params, const int num) {
			return apply<true>(src, params, num);
		}

		template <typename T> static ImageChannel distort(const ImageChannel& src, const T* params, const int num) {
			return apply<false>(src, params, num);
		}

	private:

		template <bool useUndistort, typename T> static ImageChannel apply(const ImageChannel& src, const T* params, const int num) {

			const int w = src.getWidth();
			const int h = src.getHeight();

			const Point2f size((float)w, (float)h);
			const Point2f offset(0.5f, 0.5f);

			ImageChannel dst(w, h);

			for (int x = 0; x < w; ++x) {
				for (int y = 0; y < h; ++y) {

					// map (x,y) to the range [-0.5;+0.5]
					const Point2f p1 = Point2f((float)x, (float)y) / size - offset;

					// undistort/distort and map back to range [0:w] / [0:h]
					const Point2f p2 = (useUndistort)
							? (undistort(p1, params, num) + offset) * size
							: (distort(p1, params, num) + offset) * size;

					// out of bounds?
					if (std::ceil(p2.x) >= w || std::ceil(p2.y) >= h) {continue;}
					if (p2.x < 0 || p2.y < 0) {continue;}

					// apply
					dst.set(x,y, Interpolation::bilinear(src,p2.x,p2.y));

				}
			}

			// done
			return dst;

		}


	};

}

#endif // LENSDISTORTIONRADIAL_H
