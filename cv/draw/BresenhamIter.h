#ifndef BRESENHAMITER_H
#define BRESENHAMITER_H

#include "../../geo/Point2.h"

namespace K {

	class BresenhamIter {

	private:

		int x1;
		int y1;

		const int x2;
		const int y2;

		int dx;
		int dy;

		int sx;
		int sy;

		int err;
		int e2;

	public:

		/** ctor with the line to iterate */
		BresenhamIter(const int x1, const int y1, const int x2, const int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {

			dx =  std::abs(x2-x1);
			sx = (x1<x2 ? 1 : -1);
			dy = -std::abs(y2-y1);
			sy = (y1<y2 ? 1 : -1);
			err = dx+dy;

		}

		/** next point available or end reached? */
		bool hasNext() const {
			return (x1!=x2) || (y1!=y2);
		}

		/** get the next point */
		const Point2i next() {
			const Point2i res(x1,y1);
			e2 = 2*err;
			if (e2 > dy) { err += dy; x1 += sx; }
			if (e2 < dx) { err += dx; y1 += sy; }
			return res;
		}

	};

}

#endif // BRESENHAMITER_H
