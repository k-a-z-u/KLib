#ifndef K_CV_FILTER_TRANSFORM_H
#define K_CV_FILTER_TRANSFORM_H

#include <eigen3/Eigen/Dense>
#include "../ImageChannel.h"
#include "../filter/Interpolation.h"

namespace K {

	namespace CV {

		/**
		 * Image transformation
		 * e.g. by applying an affine transformation matrix
		 */
		class Transform {

		public:

			template <typename Scalar> static void affine(const Eigen::Matrix<Scalar,3,3> matrix, const ImageChannel& src, ImageChannel& dst) {

				// src width/height
				const int sw = src.getWidth();
				const int sh = src.getHeight();

				// temporal locations
				Eigen::Matrix<Scalar,3,1> vSrc; vSrc << 0,0,1;
				Eigen::Matrix<Scalar,3,1> vDst;

				// process each pixel
				for (vSrc(1) = 0; vSrc(1) < dst.getHeight(); ++vSrc(1)) {
					for (vSrc(0) = 0; vSrc(0) < dst.getWidth(); ++vSrc(0)) {
						transform(matrix, vSrc, vDst);
						if (vDst(0) < 0 || vDst(1) < 0)		{continue;}
						if (vDst(0) >= sw || vDst(1) >= sh)	{continue;}
						dst.set(vSrc(0), vSrc(1), sample(vDst(0), vDst(1), src));
					}
				}



			}

		private:

			static inline float sample(const float x, const float y, const ImageChannel& src) {
				return cubic(x,y,src);
			}

			static inline float none(const float x, const float y, const ImageChannel& src) {
				return Interpolation::none(src,x,y);
			}

			static inline float bilinear(const float x, const float y, const ImageChannel& src) {
				return Interpolation::bilinear(src,x,y);
			}

			static inline float cubic(const float x, const float y, const ImageChannel& src) {
				return Interpolation::bicubic(src,x,y);
			}

			static inline float multisample(const float x, const float y, const ImageChannel& src) {

				float size = 0.9f;
				float step = 0.1f;

				float sum = 0;
				float cnt = 0;

				for (float dx = -size; dx <= +size; dx += step) {
					for (float dy = -size; dy <= +size; dy += step) {
						const float dist = 1.0f / (1 + std::sqrt(dx*dx + dy*dy));
						sum += Interpolation::none(src, x+dx, y+dy) * dist;
						cnt += dist;
					}
				}

				return sum/cnt;

			}



			// transform (sx,sy,1) into (dx,dy,1) using the provided matrix
			template <typename Mat, typename Vec> static inline void transform(const Mat& mat, const Vec& src, Vec& dst) {
				dst = mat * src;
				dst /= dst(2);
			}


		};

	}

}

#endif // K_CV_FILTER_TRANSFORM_H
