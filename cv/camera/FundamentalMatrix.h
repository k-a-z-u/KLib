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
	 */
	class FundamentalMatrix {


	private:

		using Scalar = double;

		Eigen::Matrix<Scalar,3,3> F;

		std::vector<Point2f> img1;
		std::vector<Point2f> img2;

	public:

		/** remove all correspondences */
		void reset() {
			img1.clear();
			img2.clear();
		}


		/** add a known correspondence between img1(x,y) and img2(x,y) */
		void addCorrespondence(const float x1, const float y1, const float x2, const float y2) {
			img1.push_back(Point2f(x1,y1));
			img2.push_back(Point2f(x2,y2));
		}


		const Point2f getImg1(const int idx) {return img1[idx];}
		const Point2f getImg2(const int idx) {return img2[idx];}


		/** estimate the Homography based on previously added correspondences */
		void estimate() {

			// normalize poth point-sets (zero-mean, std-dev=1)
			normalize(img1);
			normalize(img2);

			// create the A-Matrix (n*9) of linear-equation-system A*x=0
			Eigen::Matrix<Scalar, Eigen::Dynamic, 9> A;
			A.conservativeResize(img1.size(), Eigen::NoChange);

			// fill the A-Matrix with one equation per point-correspondence
			for (int i = 0; i < (int) img1.size(); ++i) {

				const float x1 = img1[i].x;
				const float y1 = img1[i].y;
				const float x2 = img2[i].x;
				const float y2 = img2[i].y;

				Eigen::Matrix<Scalar, 1, 9> row;
				row << (x1*x2), (x1*y2), (x1), (y1*x2), (y1*y2), (y1), (x2), (y2), 1;
				A.row(i) = row;

			}

			// https://www.youtube.com/watch?v=QzYn0OPO0Yw#t=0.080562

			// calculate SVD for A
			Eigen::JacobiSVD<decltype(A)> svdA;
			svdA.compute(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
			Eigen::JacobiSVD<decltype(A)>::MatrixVType V = svdA.matrixV();

			// V is a 9x9 matrix. For F, we use the column, corresponding to the smallest singular value in D
			const int smlIdx = getMinIdx(svdA.singularValues());
			_assertEqual(V.cols()-1, smlIdx, "should always be the last index of the last column");
			Eigen::Matrix<Scalar,9,1> F = V.col(smlIdx);

			// reshape from 9x1 to 3x3
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

			// estimate the epipoles

			{

				Eigen::Matrix<Scalar,3,3> FF = this->F * this->F.transpose();

				Eigen::SelfAdjointEigenSolver<decltype(FF)> solver(FF);

				solver.eigenvalues()

			}



		}



		/** get the fundamental matrix */
		Eigen::Matrix<Scalar,3,3> get() const {return F;}

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
