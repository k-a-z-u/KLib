/*
 * Point.h
 *
 *  Created on: Jul 4, 2013
 *      Author: Frank Ebner
 */

#ifndef POINT_H_
#define POINT_H_

#include <iosfwd>
#include <math.h>

namespace K {

// forward declaration
struct Line;

/** 2D Point */
struct Point {

	/** x coordinate */
	double x;

	/** y coordinate */
	double y;

	/** ctor */
	Point() : x(0), y(0) {;}

	/** ctor */
	Point(double x, double y) : x(x), y(y) {;}

	/** is this point a multiple of another point? */
	bool isMultipleOf(const Point& other) const {
		double factX = x / other.x;
		double factY = y / other.y;
		return factX == factY;
	}

	/** get a ray spawned from this point using the provided length and direction (in radians) */
	Line getRay(double radians, double length) const;

	/** set the coordinates */
	void set(double x, double y) {
		this->x = x;
		this->y = y;
	}

	/** get distance between two points */
	double getDistance(const Point& p) const {
		return sqrt( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) );
	}


	Point& operator += (const Point& right) {this->x += right.x; this->y += right.y; return *this;}

	Point& operator /= (double right) {this->x /= right; this->y /= right; return *this;}

	Point operator + (const Point& right) const {
		return Point(this->x+right.x, this->y+right.y);
	}

	Point operator * (double right) const {
		return Point((this->x*right), (this->y*right));
	}

	bool operator == (const Point& other) const {
		return (x == other.x) && (y == other.y);
	}

};

/** ofstream */
std::ostream& operator<< (std::ostream& out, Point& p);

}

#endif /* POINT_H_ */
