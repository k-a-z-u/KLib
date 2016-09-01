#ifndef K_GEP_POINT3_H
#define K_GEP_POINT3_H

#include <cmath>
#include <cstdint>

#include <eigen3/Eigen/Dense>

namespace K {

	/** point in 3D space */
	template <typename T> struct Point3 {

		/** x-coordinate */
		T x;

		/** y-coordinate */
		T y;

		/** z-coordinate */
		T z;


		/** empty ctor */
		Point3() : x(0), y(0), z(0) {;}

		/** ctor */
		Point3(const T x, const T y, const T z) : x(x), y(y), z(z) {;}


		/** get distance between two points */
		template <typename T2> T getDistance(const Point3<T2>& p) const {
			return std::sqrt( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) + (z-p.z)*(z-p.z) );
		}

		/** get the length when pretending the point to be a vector from (0,0) */
		T getLength() const {
			return std::sqrt( x*x + y*y + z*z );
		}


		Point3& operator += (const Point3& right) {this->x += right.x; this->y += right.y; this->z += right.z; return *this;}

		Point3& operator -= (const Point3& right) {this->x -= right.x; this->y -= right.y; this->z -= right.z; return *this;}

		Point3& operator *= (const Point3& right) {this->x *= right.x; this->y *= right.y; this->z *= right.z; return *this;}

		Point3& operator /= (const Point3& right) {this->x /= right.x; this->y /= right.y; this->z /= right.z; return *this;}


		Point3& operator *= (const T right) {this->x *= right; this->y *= right; this->z *= right; return *this;}

		Point3& operator /= (const T right) {this->x /= right; this->y /= right; this->z /= right; return *this;}


		Point3 operator - () const {return Point3(-this->x, -this->y, -this->z);}


		Point3 operator + (const Point3& right) const {return Point3(this->x+right.x, this->y+right.y, this->z+right.z);}

		Point3 operator - (const Point3& right) const {return Point3(this->x-right.x, this->y-right.y, this->z-right.z);}

		Point3 operator * (const Point3& right) const {return Point3(this->x*right.x, this->y*right.y, this->z*right.z);}

		Point3 operator / (const Point3& right) const {return Point3(this->x/right.x, this->y/right.y, this->z/right.z);}


		Point3 operator * (const T right) const {return Point3((this->x*right), (this->y*right), (this->z*right));}

		Point3 operator / (const T right) const {return Point3((this->x/right), (this->y/right), (this->z/right));}


		bool operator == (const Point3& other) const {return (x == other.x) && (y == other.y) && (z == other.z);}


		/** dot-product between this point and the given one */
		float dot(const Point3& o) const {return x*o.x + y*o.y + z*o.z;}

		/** cross-product between this point and the given one */
		Point3 cross(const Point3& o) const {
			const float x = y*o.z - z*o.y;
			const float y = z*o.x - x*o.z;
			const float z = x*o.y - y*o.x;
			return Point3(x,y,z);
		}

		void normalize() {
			const float len = getLength();
			x /= len;
			y /= len;
			z /= len;
		}


		Point3 normalized() const {
			const T len = getLength();
			return (Point3(x/len, y/len, z/len));
		}

		/** get (abs(x), abs(y), abs(z)) */
		Point3 getAbs() const {
			return Point3(std::abs(x), std::abs(y), std::abs(z));
		}

		std::string asString() const {return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(y) + ")";}


		/** multiply by libEigen matrix */
		template <typename Scalar> Point3<Scalar> friend operator * (const Eigen::Matrix<Scalar, 3, 3>& mat, const Point3<Scalar>& pt) {
			Eigen::Matrix<Scalar, 3, 1> vec;
			vec << pt.x, pt.y, pt.z;
			vec = mat * vec;
			return Point3<Scalar>(vec(0), vec(1), vec(2));
		}


	};

	typedef Point3<float> Point3f;
	typedef Point3<int> Point3i;

}



#endif // K_GEP_POINT3_H
