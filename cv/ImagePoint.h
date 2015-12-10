#ifndef IMAGEPOINT_H
#define IMAGEPOINT_H

namespace K {

	struct ImagePoint {

		int x;
		int y;

		/** ctor */
		ImagePoint(const int x, const int y) : x(x), y(y) {}

		/** get the distance between two points */
		float distance(const ImagePoint& o) const {
			const int dx = x - o.x;
			const int dy = y - o.y;
			return (float) std::sqrt(dx*dx + dy*dy);
		}

	};

}

#endif // IMAGEPOINT_H

