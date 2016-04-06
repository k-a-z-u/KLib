#ifndef K_CV_CAMERA_FUNDAMENTALMATRIX_H
#define K_CV_CAMERA_FUNDAMENTALMATRIX_H

#include "../Point2.h"
#include <vector>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

#include "../ImageChannel.h"
#include "../filter/Transform.h"

#include "../../Assertions.h"

namespace K {


	/**
	 * estimate the fundamental-matrix F
	 * satisfying x * F * x' = 0 for
	 * several given x,x' correspondences
	 *
	 *             | a b c |   |xl|
	 * |xr yr 1| * | d e f | * |yl| = 0
	 *             | g h i |   |1 |
	 *
	 *             | axl + byl + c |
	 * |xr yr 1| * | dxl + eyl + f |
	 *             | gxl + hyl + i |
	 *
	 * axlxr + bylxr + cxr +
	 * dxlyr + eylyr + fyr +
	 * gxl + hyl + i = 0
	 *
	 */
	class FundamentalMatrix {


	private:

		using Scalar = double;

		using Vec2 = Eigen::Matrix<Scalar,2,1>;
		using Vec3 = Eigen::Matrix<Scalar,3,1>;

		Eigen::Matrix<Scalar,3,3> F;

		std::vector<Point2f> imgLeft;
		std::vector<Point2f> imgRight;

	public:

		/** remove all correspondences */
		void reset() {
			imgLeft.clear();
			imgRight.clear();
		}


		/** add a known correspondence between imgLeft(x,y) and imgRight(x,y) */
		void addCorrespondence(const float xl, const float yl, const float xr, const float yr) {
			imgLeft.push_back(Point2f(xl, yl));
			imgRight.push_back(Point2f(xr, yr));
		}

		/** add a known correspondence between imgLeft(x,y) and imgRight(x,y) */
		template <typename T> void addCorrespondence(const Point2<T> pl, const Point2<T> pr) {
			imgLeft.push_back(Point2f(pl.x, pl.y));
			imgRight.push_back(Point2f(pr.x, pr.y));
		}

		/** get the idx-th correspondence point within the left image */
		const Point2f getPointLeft(const int idx) const {return imgLeft[idx];}

		/** get the idx-th correspondence point within the right image */
		const Point2f getPointRight(const int idx) const {return imgRight[idx];}


		/** estimate the Homography based on previously added correspondences */
		void estimate() {

			// normalize poth point-sets (zero-mean, std-dev=1)
//			normalize(img1);
//			normalize(img2);

			// create the A-Matrix (n*9) of linear-equation-system A*x=0
			Eigen::Matrix<Scalar, Eigen::Dynamic, 9> A;
			A.conservativeResize(imgLeft.size(), Eigen::NoChange);

			// fill the A-Matrix with one equation per point-correspondence
			for (int i = 0; i < (int) imgLeft.size(); ++i) {

				const float xr = imgRight[i].x;
				const float yr = imgRight[i].y;
				const float xl = imgLeft[i].x;
				const float yl = imgLeft[i].y;

				Eigen::Matrix<Scalar, 1, 9> row;
				row << (xr*xl), (xr*yl), (xr), (yr*xl), (yr*yl), (yr), (xl), (yl), 1;
				A.row(i) = row;

			}

			// https://www.youtube.com/watch?v=QzYn0OPO0Yw#t=0.080562

			// calculate SVD for A
			Eigen::JacobiSVD<decltype(A)> svdA;
			svdA.compute(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
			Eigen::JacobiSVD<decltype(A)>::MatrixVType V = svdA.matrixV();

			// V is a 9x9 matrix. For F, we use the column, corresponding to the smallest singular value in D
			const int smlIdx = getMinIdx(svdA.singularValues());
			_assertEqual(svdA.singularValues().rows()-1, smlIdx, "should always be the index of the last row");
			Eigen::Matrix<Scalar,9,1> F = V.col(smlIdx);

			// reshape from 9xl to 3x3
			Eigen::Matrix<Scalar,3,3> FF(F.data());

			// why do we need this step?
			{

				// calculate SVD for FF
				Eigen::JacobiSVD<decltype(FF)> svdFF;
				svdFF.compute(FF, Eigen::ComputeFullU | Eigen::ComputeFullV);
				Eigen::JacobiSVD<decltype(FF)>::MatrixUType FU = svdFF.matrixU();
				Eigen::JacobiSVD<decltype(FF)>::MatrixVType FV = svdFF.matrixV();
				Eigen::JacobiSVD<decltype(FF)>::SingularValuesType FD = svdFF.singularValues();

				// zero-out the smallest value in FD
				std::cout << FD << std::endl << std::endl;
				if (FD(0) < FD(1)) {
					if (FD(0) < FD(2)) {FD(0) = 0;} else {FD(2) = 0;}
				} else {
					if (FD(1) < FD(2)) {FD(1) = 0;} else {FD(2) = 0;}
				}
				std::cout << FD << std::endl << std::endl;

				// D is a vector, despite it should be a diagonal matrix -> reshape it here
				Eigen::Matrix<Scalar,3,3> FFF = FU * FD.asDiagonal() * FV.transpose();

				// ensure the matrix is normalized
				std::cout << FF << std::endl << std::endl;
				std::cout << FFF << std::endl << std::endl;
				std::cout << FFF.norm() << std::endl << std::endl;
				FFF = FFF / FFF.norm();
				std::cout << FFF << std::endl;

				// done
				this->F = FFF;

			}

			{

				const Eigen::Matrix<Scalar,3,3> FF = this->F * this->F.transpose();
				Eigen::SelfAdjointEigenSolver<decltype(FF)> solver(FF);

				const Eigen::Matrix<Scalar,1,3> vec = getMin(solver.eigenvectors(), solver.eigenvalues());

				int i = 0;

			}



		}

		/** get the index of the smallest element within the given vector */
		template <typename Vec> int getMin(const Vec vec) {
			int minI = -1;
			float minV = INFINITY;
			for (int i = 0; i < vec.rows(); ++i) {
				if (vec(i) < minV) {minV = vec(i); minI = i;}
			}
			return minI;
		}

		/** get the eigenvector for the samllest eigenvalue */
		template <typename Mat, typename Vec> Vec getMin(const Mat evec, const Vec eval) {
			const int minIdx = getMin(eval);
			return evec.col(minIdx);
		}

		/** get the fundamental matrix */
		Eigen::Matrix<Scalar,3,3> getFundamentalMatrix() const {return F;}

		/**
		 * get the equation for an epi-line within the right-image,
		 * using the given point in the left image.
		 * l_r = F * p_l
		 * the equation is given as 3 parameters A,B,C denoting Ax + By + C = 0.
		 * to transform this into a line, just rephrase the problem as y = mx+b:
		 * Ax + By + C = 0
		 * By = -Ax - C
		 * y = -(Ax + C) / B
		 */
		Eigen::Matrix<Scalar,3,1> getEpilineRight(const Vec3& pLeft) const {
			Vec3 v3 = F.transpose() * pLeft;	// SHOULD BE WITHOUT TRANSPOSE?!
			Vec2 v2; v2 << v3(0), v3(1);		// only the first two coordinates
			return v3 / v2.norm();				// normalize using only the first two coordinates (scales into image-space)
		}

		template <typename T> Eigen::Matrix<Scalar,3,1> getEpilineRight(Point2<T> pLeft) const {
			return getEpilineRight(Vec3(pLeft.x, pLeft.y, 1));
		}

		/**
		 * get the equation for an epi-line within the left-image,
		 * using the given point in the right image.
		 * l_l = F^T * p_r
		 * see getEpilineRight()
		 */
		Eigen::Matrix<Scalar,3,1> getEpilineLeft(const Vec3& pRight) const {
			Vec3 v3 = F * pRight;	// // SHOULD BE WITH TRANSPOSE?!
			Vec2 v2; v2 << v3(0), v3(1);		// only the first two coordinates
			return v3 / v2.norm();				// normalize using only the first two coordinates (scales into image-space)
		}


		/** get the index of the smallest value in vec */
		template <typename T> int getMinIdx(T vec) const {
			Scalar min = 99999999;
			int idx = -1;
			for (int i = 0; i < vec.rows(); ++i) {
				if (vec(i) < min) {min = vec(i); idx = i;}
			}
			return idx;

		}

		/**
		 * normalize the given point-set:
		 * average = 0;
		 * std-dev = 1
		 */
		void normalize(std::vector<Point2f>& pts) const {

			Point2f sum(0,0);
			Point2f sum2(0,0);
			const int cnt = (int) pts.size();

			// calculate the average and std-dev for the point-set
			for (const Point2f p : pts) {
				sum += p;
				sum2 += p*p;
			}

			// calculate average, variance and sigma
			const Point2f avg = sum / (float)cnt;
			const Point2f var =  (sum2 / (float)cnt) - (avg*avg);
			const Point2f sigma( std::sqrt(var.x), std::sqrt(var.y) );

			// normalize each point
			for (Point2f& p : pts) {
				p -= avg;
				p /= sigma;
			}

		}

	};

}

#endif // K_CV_CAMERA_FUNDAMENTALMATRIX_H
