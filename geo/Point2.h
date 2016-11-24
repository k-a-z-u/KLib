#ifndef POINT2_H
#define POINT2_H

#include <cmath>
#include <cstdint>
#include <string>

namespace K {

	/** point in 2D space */
	template <typename T> struct Point2 {

		/** x-coordinate */
		T x;

		/** y-coordinate */
		T y;

		/** empty ctor */
		Point2() : x(0), y(0) {;}

		/** ctor */
		Point2(const T x, const T y) : x(x), y(y) {;}


		/** get distance between two points */
		template <typename T2> T getDistance(const Point2<T2>& p) const {
			return std::sqrt( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) );
		}

		/** get the length when pretending the point to be a vector from (0,0) */
		T getLength() const {
			return std::sqrt( x*x + y*y );
		}

		/** treat this point as a vector from (0,0) and rotate it by the angle theta */
		void rotate(const float theta) {
			const T _x = (x*std::cos(theta)) - (y*std::sin(theta));
			const T _y = (x*std::sin(theta)) + (y*std::cos(theta));
			this->x = _x;
			this->y = _y;
		}

		/** treat this point as a vector from (0,0) and return a version rotated by the angle theta */
		Point2 getRotated(const float theta) const {
			const float _x = (x*std::cos(theta)) - (y*std::sin(theta));
			const float _y = (x*std::sin(theta)) + (y*std::cos(theta));
			return Point2(_x, _y);
		}

		/** get (abs(x), abs(y)) */
		Point2 getAbs() const {
			return Point2(std::abs(x), std::abs(y));
		}

		T getDirection() const {
			return std::atan2(y,x);
		}



		Point2& operator += (const Point2& right) {this->x += right.x; this->y += right.y; return *this;}

		Point2& operator -= (const Point2& right) {this->x -= right.x; this->y -= right.y; return *this;}

		Point2& operator *= (const Point2& right) {this->x *= right.x; this->y *= right.y; return *this;}

		Point2& operator /= (const Point2& right) {this->x /= right.x; this->y /= right.y; return *this;}


		Point2& operator *= (const T right) {this->x *= right; this->y *= right; return *this;}

		Point2& operator /= (const T right) {this->x /= right; this->y /= right; return *this;}


		Point2 operator - () const {return Point2(-this->x, -this->y);}


		Point2 operator + (const Point2& right) const {return Point2(this->x+right.x, this->y+right.y);}

		Point2 operator - (const Point2& right) const {return Point2(this->x-right.x, this->y-right.y);}

		Point2 operator * (const Point2& right) const {return Point2(this->x*right.x, this->y*right.y);}

		Point2 operator / (const Point2& right) const {return Point2(this->x/right.x, this->y/right.y);}


		Point2 operator * (const T right) const {return Point2((this->x*right), (this->y*right));}

		Point2 operator / (const T right) const {return Point2((this->x/right), (this->y/right));}


		bool operator == (const Point2& other) const {return (x == other.x) && (y == other.y);}

		std::string asString() const {return "(" + std::to_string(x) + "," + std::to_string(y) + ")";}


		void normalize() {
			const float len = getLength();
			x /= len;
			y /= len;
		}

		/** normalizing works only for floats/doubles */
		Point2<float> normalized() {
			const float len = getLength();
			return Point2<float>(x/len, y/len);
		}

		/** cast. e.g. from int to float or vice versa */
		template <typename T2> operator Point2<T2> () const {
			return Point2<T2>(x, y);
		}

		friend float dot(const Point2 p1, const Point2 p2) {
			return (p1.x * p2.x) + (p1.y * p2.y);
		}

		friend Point2 sqrt(const Point2 p) {
			return Point2(std::sqrt(p.x), std::sqrt(p.y));
		}

	};

	typedef Point2<float> Point2f;
	typedef Point2<int> Point2i;




}



namespace std {
	template<typename T> struct hash<K::Point2<T>> {
		size_t operator() (const K::Point2<T>& p) const {
			//const uint32_t x = *((uint32_t*)(&p.x));
			//const uint32_t y = *((uint32_t*)(&p.y));
			//return (x ^ y);
			return (p.x * 13) + (p.y * 41);
		}
	};
}

#endif // POINT2_H
