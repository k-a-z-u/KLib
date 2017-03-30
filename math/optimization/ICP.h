#ifndef ICP_H
#define ICP_H

#include <vector>

#include "../../geo/Point3.h"

#include "../../geo/Matrix.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

namespace K {

	/**
	 * simple ICP solver
	 * http://www.markuszancolo.at/2015/12/icp-with-svd-hands-on/
	 */
	class ICP {

	public:

		/**
		 * calculate matrix M describing:		p2 = M * p1
		 */
		template <typename T> static Mat4f solve(const std::vector<Point3<T>>& set1, const std::vector<Point3<T>>& set2) {

			const int cnt = (int) set1.size();

			// calculate mean
			Point3<T> sum1;
			Point3<T> sum2;
			for (int i = 0; i < cnt; ++i) {
				sum1 += set1[i];
				sum2 += set2[i];
			}
			Point3<T> avg1 = sum1 / (float)cnt;
			Point3<T> avg2 = sum2 / (float)cnt;

			// move each set to its mean
			std::vector<Point3<T>> centered1; centered1.resize(set1.size());
			std::vector<Point3<T>> centered2; centered2.resize(set2.size());

			for (int i = 0; i < cnt; ++i) {
				centered1[i] = set1[i] - avg1;
				centered2[i] = set2[i] - avg2;
			}

			// calculate M = sum(v1_i * v2_i')
			Mat3f M = Mat3f::Zero();
			for (int i = 0; i < cnt; ++i) {
				const Vec3f v1(centered1[i].x, centered1[i].y, centered1[i].z);
				const Vec3f v2(centered2[i].x, centered2[i].y, centered2[i].z);

				M += v1 * v2.transpose();
			}

			// calculate SVD(M)
			Eigen::JacobiSVD<Mat3f> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);

			// build the final rot ation matrix
			Mat3f rot3 = (svd.matrixV()*(svd.matrixU().transpose())).transpose();
			Mat4f rot4; rot4 <<
				rot3(0,0),	rot3(0,1),	rot3(0,2),	0,
				rot3(1,0),	rot3(1,1),	rot3(1,2),	0,
				rot3(2,0),	rot3(2,1),	rot3(2,2),	0,
				0,			0,			0,			1;

			// shifting offset
			// TODO ORDER?!
			Mat4f tra1 = MatrixHelper::getTranslation(-avg2.x, -avg2.y, -avg2.z);
			Mat4f tra2 = MatrixHelper::getTranslation(+avg1.x, +avg1.y, +avg1.z);

			return (tra2*rot4*tra1).inverse();

		}


		/**
		 * A matrix that converts all points P to another point-set P':		 *
		 *		A * p1 = p1'
		 *		A * p2 = p2'
		 *		A * p3 = p3'
		 *		...
		 *
		 * which gives us many individual formulas:
		 *		p1'.x = a * p1.x + b * p1.y + c * p1.z
		 *		p1'.y = d * p1.x + e * p1.y + f * p1.z
		 *
		 * build a matrix from pointset_1
		 * calculate its pseude-inverse
		 * multiply by the values of pointset_2
		 * -> least-squares error
		 *
		 */
		// THIS IS NOT A NORMAL ICP AS IT ALSO ADJUSTS THE SCALE!
//		template <typename T> static Mat3f solve(std::vector<Point3<T>>& set1, std::vector<Point3<T>>& set2) {

//			Eigen::Matrix<float, Eigen::Dynamic, 9> A;
//			Eigen::Matrix<float, Eigen::Dynamic, 1> b;

//			for (int i = 0; i < (int) set1.size(); ++i) {

//				const Point3<T>& p1 = set1[i];

//				Eigen::Matrix<float, 1, 9> row1;	row1 << p1.x, p1.y, p1.z,	0, 0, 0,			0, 0, 0;
//				Eigen::Matrix<float, 1, 9> row2;	row2 << 0, 0, 0,			p1.x, p1.y, p1.z,	0, 0, 0;
//				Eigen::Matrix<float, 1, 9> row3;	row3 << 0, 0, 0,			0, 0, 0,			p1.x, p1.y, p1.z;

//				A.conservativeResize(A.rows() + 3, Eigen::NoChange);

//				A.row(A.rows()-3) = row1;
//				A.row(A.rows()-2) = row2;
//				A.row(A.rows()-1) = row3;

//				b.conservativeResize(b.rows() + 3, Eigen::NoChange);
//				const Point3<T>& p2 = set2[i];
//				b(b.rows()-3) = p2.x;
//				b(b.rows()-2) = p2.y;
//				b(b.rows()-1) = p2.z;

//			}

//			auto pseudoInv = (A.transpose() * A).inverse() * A.transpose();
//			Eigen::Matrix<float, 9, 1> x = pseudoInv * b;

//			Eigen::Matrix<float, 3, 3> mat;
//			mat << x(0), x(1), x(2), x(3), x(4), x(5), x(6), x(7), x(8);
//			return mat;

//		}

	};

}

#endif // ICP_H
