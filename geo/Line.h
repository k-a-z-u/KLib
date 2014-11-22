/*
 * Line.h
 *
 *  Created on: Jul 4, 2013
 *      Author: Frank Ebner
 */

#ifndef LINE_H_
#define LINE_H_

#include "Point.h"
#include <math.h>

namespace K {

/**
 * represents one line using start- and end-point
 */
struct Line {

	/** starting point */
	Point p1;

	/** ending point */
	Point p2;

	/** ctor */
	Line() : p1(0,0), p2(0,0) {;}

	/** ctor */
	Line(double x1, double y1, double x2, double y2) : p1(x1,y1), p2(x2,y2) {;}

	/** ctor */
	Line(const Point& p1, const Point& p2) : p1(p1), p2(p2) {;}

	/** modify the line */
	void set(const double x1, const double y1, const double x2, const double y2) {
		p1.set(x1,y1);
		p2.set(x2,y2);
	}

	/** get a line, orthogonal to this one */
	Line getOrthogonal() const {
		return getOrthogonal(p1);
	}

	/** get a line, orthogonal to this one and starting at src */
	Line getOrthogonal(const Point src) const {
		const Point t1 = p2 - p1;	// move to origin of this line (0,0)
		Point t2(t1.y, -t1.x);		// make orthogonal to this line
		t2 += src;					// move to the desired src
		return Line(src, t2);
	}

	//	/** get the incline of this line */
	//	float getIncline() const {
	//		return (p2.y - p1.y) / (p2.x - p1.x);
	//	}

	/** get the angle between start and end of this line */
	double getAngle() const {

		double dstA = p2.x - p1.x;
		double dstG = p2.y - p1.y;

		if (dstA == 0 && dstG == 0) {return 0;}

		return atan2(dstG, dstA);

	}

	/** get the length of this line */
	double getLength() const {
		return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) );
	}

	double getDeltaX() const {
		return p1.x - p2.x;
	}

	double getDeltaY() const {
		return p1.y - p2.y;
	}

	/** get the direction vector for this line */
	Point getDirVec() const {
		return Point(p2.x - p1.x, p2.y - p1.y);
	}

	/** scale the line by the given factor */
	Line getScaled(float factor) {
		Point t1 = p1; t1 -= p2; t1 *= factor; t1 += p2;
		Point t2 = p2; t2 -= p1; t2 *= factor; t2 += p1;
		return Line(t1, t2);
	}

	//	/** is this line parallel to another one? */
	//	bool isParallelTo(const Line& other) const {
	//		Point v1 = getDirVec();
	//		Point v2 = other.getDirVec();
	//		return v1.isMultipleOf(v2);
	//	}

	/** get intersection between these two lines (unlimited length!) */
	bool getLineIntersection(const Line& other, Point& result) const {

		double bx = p2.x - p1.x;
		double by = p2.y - p1.y;

		double dx = other.p2.x - other.p1.x;
		double dy = other.p2.y - other.p1.y;

		double b_dot_d_perp = bx*dy - by*dx;

		if(b_dot_d_perp == 0) {return false;}

		double cx = other.p1.x - p1.x;
		double cy = other.p1.y - p1.y;
		double t = (cx*dy - cy*dx) / b_dot_d_perp;

		result.x = p1.x + t * bx;
		result.y = p1.y + t * by;

		return true;

	}

	/** get intersection between these two line-segments (limited length!) */
	bool getSegmentIntersection(const Line& other, Point& result) const {

		double bx = p2.x - p1.x;
		double by = p2.y - p1.y;

		double dx = other.p2.x - other.p1.x;
		double dy = other.p2.y - other.p1.y;

		double b_dot_d_perp = bx*dy - by*dx;

		if(b_dot_d_perp == 0) {return false;}

		double cx = other.p1.x - p1.x;
		double cy = other.p1.y - p1.y;

		double t = (cx * dy - cy * dx) / b_dot_d_perp;
		if(t < 0 || t > 1) {return false;}

		double u = (cx * by - cy * bx) / b_dot_d_perp;
		if(u < 0 || u > 1) {return false;}

		result.x = p1.x + t * bx;
		result.y = p1.y + t * by;

		return true;

	}

	bool operator == (const Line& other) const {
		return (p1 == other.p1) && (p2 == other.p2);
	}

};

}

#endif /* LINE_H_ */
