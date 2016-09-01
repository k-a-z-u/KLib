#ifndef K_MATRIX_H
#define K_MATRIX_H

#include "Point3.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

typedef Eigen::Matrix<float, 3, 1> Vec3f;
typedef Eigen::Matrix<float, 4, 1> Vec4f;
typedef Eigen::Matrix<float, 6, 1> Vec6f;

typedef Eigen::Matrix<int, 3, 1> Vec3i;
typedef Eigen::Matrix<int, 4, 1> Vec4i;
typedef Eigen::Matrix<int, 6, 1> Vec6i;

typedef Eigen::Matrix<float, 3, 3> Mat3f;
typedef Eigen::Matrix<float, 4, 4> Mat4f;


namespace K {

	class MatrixHelper {

	public:

		/** get a rotation matrix for the given x,y,z rotation (in radians) */
		static Mat4f getRotation(const Vec3f& rotation) {
			const float g = rotation.x(); const float b = rotation.y(); const float a = rotation.z();
			const float a11 = std::cos(a)*std::cos(b);
			const float a12 = std::cos(a)*std::sin(b)*std::sin(g)-std::sin(a)*std::cos(g);
			const float a13 = std::cos(a)*std::sin(b)*std::cos(g)+std::sin(a)*std::sin(g);
			const float a21 = std::sin(a)*std::cos(b);
			const float a22 = std::sin(a)*std::sin(b)*std::sin(g)+std::cos(a)*std::cos(g);
			const float a23 = std::sin(a)*std::sin(b)*std::cos(g)-std::cos(a)*std::sin(g);
			const float a31 = -std::sin(b);
			const float a32 = std::cos(b)*std::sin(g);
			const float a33 = std::cos(b)*std::cos(g);
			Mat4f m;
			m <<
				a11,	a12,	a13,	0,
				a21,	a22,	a23,	0,
				a31,	a32,	a33,	0,
				0,		0,		0,		1
			;
			return m;
		}

		/** get a translation matrix */
		static Mat4f getTranslation(const Vec3f& pos) {
			return getTranslation(pos.x(), pos.y(), pos.z());
		}

		/** get a translation matrix */
		static Mat4f getTranslation(const float x, const float y, const float z) {
			Mat4f m = Mat4f::Identity();
			m(0, 3) = x;
			m(1, 3) = y;
			m(2, 3) = z;
			return m;
		}

		/** get a matrix that rotates the vector "from" into the vector "to" */
		static Eigen::Matrix3f getRotationMatrix(const Eigen::Vector3f& from, const Eigen::Vector3f& to) {

			// http://math.stackexchange.com/questions/293116/rotating-one-3d-vector-to-another

			if (from ==  to) { return Eigen::Matrix3f::Identity(); }
			if (from == -to) { return Eigen::Matrix3f::Identity(); }

			const Eigen::Vector3f x = from.cross(to) / from.cross(to).norm();

			const float angle = std::acos( from.dot(to) / from.norm() / to.norm() );

			Eigen::Matrix3f A; A <<
				0,		-x(2),	x(1),
				x(2),	0,		-x(0),
				-x(1),	x(0),	0;

			return Eigen::Matrix3f::Identity() + (std::sin(angle) * A) + ((1-std::cos(angle)) * (A*A));

		}

	};





	inline Point3f operator * (const Mat4f& m, const Point3f p) {
		const Vec4f v1(p.x, p.y, p.z, 1);
		const Vec4f v2 = m * v1;
		return Point3f(v2(0), v2(1), v2(2));
	}

	inline Point3f operator * (const Mat3f& m, const Point3f p) {
		const Vec3f v1(p.x, p.y, p.z);
		const Vec3f v2 = m * v1;
		return Point3f(v2(0), v2(1), v2(2));
	}

}



#endif // K_MATRIX_H
