#ifndef K_CV_DRAWER_H
#define K_CV_DRAWER_H

#include "../ImageChannel.h"
#include "../Point2.h"


namespace K {

	/**
	 * very simple helper to draw common structures onto an ImageChannel
	 */
	class Drawer {

	private:

		ImageChannel& img;
		float fg = 0.0f;
		float bg = 1.0f;

	public:

		/** ctor */
		Drawer(ImageChannel& img) : img(img) {

		}

		/** draw a rectangle */
		void drawRect(const int x1, const int y1, const int x2, const int y2) {

			drawLine(x1, y1, x2, y1);	// upper
			drawLine(x1, y2, x2, y2);	// lower

			drawLine(x1, y1, x1, y2);	// left
			drawLine(x2, y1, x2, y2);	// right

		}

		/** draw a rectangle */
		void drawRect(const Point2i p1, const Point2i p2) {
			drawRect(p1.x, p1.y, p2.x, p2.y);
		}

		/** draw a line */
		void drawLine(int x1, int y1, const int x2, const int y2) {

			const int dx =  std::abs(x2-x1), sx = (x1<x2 ? 1 : -1);
			const int dy = -std::abs(y2-y1), sy = (y1<y2 ? 1 : -1);
			int err = dx+dy, e2;

			while(1) {

				if (x1 >= 0 && y1 >= 0 && x1 < img.getWidth() && y1 < img.getHeight()) { img.set(x1, y1, fg); }
				if ( (x1==x2) && (y1==y2) ) {break;}
				e2 = 2*err;
				if (e2 > dy) { err += dy; x1 += sx; }
				if (e2 < dx) { err += dx; y1 += sy; }
			}

		}

	};

}

#endif // K_CV_DRAWER_H
