#ifndef POINT3_H
#define POINT3_H

#include <cmath>
#include <cstdint>

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


		Point3 operator + (const Point3& right) const {return Point3(this->x+right.x, this->y+right.y, this->z+right.z);}

		Point3 operator - (const Point3& right) const {return Point3(this->x-right.x, this->y-right.y, this->z-right.z);}

		Point3 operator * (const Point3& right) const {return Point3(this->x*right.x, this->y*right.y, this->z*right.z);}

		Point3 operator / (const Point3& right) const {return Point3(this->x/right.x, this->y/right.y, this->z/right.z);}

		Point3 operator * (const T right) const {return Point3((this->x*right), (this->y*right), (this->z*right));}

		Point3 operator / (const T right) const {return Point3((this->x/right), (this->y/right), (this->z/right));}


		bool operator == (const Point3& other) const {return (x == other.x) && (y == other.y) && (y == other.z);}


		/** dot-product between this point and the given one */
		float dot(const Point3& o) const {return x*o.x + y*o.y + z*o.z;}

		std::string asString() const {return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(y) + ")";}


	};

	typedef Point3<float> Point3f;
	typedef Point3<int> Point3i;

}



#endif // POINT3_H
