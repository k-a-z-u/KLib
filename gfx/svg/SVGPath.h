/*
 * SVGPath.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SVGPATH_H_
#define SVGPATH_H_

#include "SVGElement.h"
#include "../../geo/Line.h"
#include "../Color.h"


namespace K {

class SVGPath : public SVGElement {

public:

	/** ctor */
	SVGPath() {;}

	void transform(double x, double y) {
		for (Line& l : lines) {
			l.p1.x += x;
			l.p2.x += x;
			l.p1.y -= y;
			l.p2.y -= y;
		}
	}

	/** get all lines within this path */
	const std::vector<Line>& getLines() const {
		return lines;
	}

	/** get the path's line color (if any) */
	Color getLineColor() {
		return lineColor;
	}

private:


	friend class SVGLoader;

	/** all lines within the path */
	std::vector<Line> lines;

	/** the path's color (if any) */
	Color lineColor;

};

}




#endif /* SVGPATH_H_ */
