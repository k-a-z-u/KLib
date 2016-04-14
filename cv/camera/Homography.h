#ifndef K_CV_CAMERA_HOMOGRAPHY_H
#define K_CV_CAMERA_HOMOGRAPHY_H

#include "../../geo/Point2.h"
#include <vector>
#include <eigen3/Eigen/Dense>

#include "../ImageChannel.h"
#include "../filter/Transform.h"

namespace K {


	/**
	 * estimate the homography matrix H
	 * to convert world(x,y,0) to img(x,y)
	 *
	 * img = H * world
	 *
	 * solve using:
	 *	y = A * x
	 *	x = inv(A) * y
	 */
	class Homography {


	private:

		using Scalar = double;

		Eigen::Matrix<Scalar, Eigen::Dynamic, 1> y;
		Eigen::Matrix<Scalar, Eigen::Dynamic, 9> A;

		Eigen::Matrix<Scalar, 3, 3> mat;
		Eigen::Matrix<Scalar, 3, 3> invMat;

	public:

		/** remove all correspondences */
		void reset() {
			y.resize(0);
			A.resize(0, Eigen::NoChange);
		}


		/** add a known correspondence from world(x,y,0) to img(x,y) */
		void addCorrespondence(const float imgX, const float imgY, const float worldX, const float worldY) {

			y.conservativeResize(y.rows() + 2);
			A.conservativeResize(A.rows() + 2, Eigen::NoChange);

			y(y.rows()-2) = imgX;
			y(y.rows()-1) = imgY;

			Eigen::Matrix<Scalar, 1, 9> row1, row2;
			row1 << worldX,worldY,1,	0,0,0,				-imgX*worldX,-imgX*worldY,0;
			row2 << 0,0,0,				worldX,worldY,1,	-imgY*worldX,-imgY*worldY,0;
			A.row(A.rows()-2) = row1;
			A.row(A.rows()-1) = row2;

			int i = 0; (void) i;



		}

		/** transform the image */
		void undo(const ImageChannel& src, ImageChannel& dst) {
			Transform::affine(invMat, src, dst);
		}



		/** estimate the Homography based on previously added correspondences */
		void estimate() {


			const auto pseudo = A.transpose() * ((A * A.transpose()).inverse());
			const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> x = pseudo * y;

			std::cout << A << std::endl << std::endl;
			std::cout << y << std::endl << std::endl;
			std::cout << pseudo << std::endl << std::endl;


			mat << x(0), x(1), x(2), x(3), x(4), x(5), x(6), x(7), 1;
			invMat = mat.inverse();

		}


		void worldToImg(const float worldX, const float worldY, float& imgX, float& imgY) {
			transform(mat, worldX, worldY, imgX, imgY);
		}


		void imgToWorld(const float imgX, const float imgY, float& worldX, float& worldY) {
			transform(invMat, imgX, imgY, worldX, worldY);
		}

	private:

		// transform (sx,sy,1) into (dx,dy,1) using the provided matrix
		template <typename T> static inline void transform(const T& mat, const float sx, const float sy, float& dx, float& dy) {
			Eigen::Matrix<Scalar, 3, 1> src; src << sx, sy, 1;
			Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> dst = mat * src;
			dst /= dst(2);
			dx = (float) dst(0);
			dy = (float) dst(1);
		}




	};

}

#endif // K_CV_CAMERA_HOMOGRAPHY_H
