#ifndef BITMAP_H
#define BITMAP_H

#include "DataMatrix.h"
#include "../geo/Point2.h"
#include "ImageChannel.h"

namespace K {

	/** binary image (yes/no) */
	class Bitmap : private DataMatrix<bool> {

	public:

		/** ctor with image size */
		Bitmap(const int w, const int h) : DataMatrix(w, h) {;}

		/** ctor from image */
		Bitmap(const ImageChannel& img) : DataMatrix(img.getWidth(), img.getHeight()) {

			auto conv = [&] (const int x, const int y, const float val) {
				if (val > 0.5) {set(x,y);}
			};
			img.forEach(conv);

		}

		void set(const Point2i p)					{DataMatrix::set(p.x, p.y, true);}
		void set(const int x, const int y)			{DataMatrix::set(x, y, true);}

		void clear(const Point2i p)					{DataMatrix::set(p.x, p.y, false);}
		void clear(const int x, const int y)		{DataMatrix::set(x, y, false);}

		bool isSet(const Point2i p) const			{return get(p.x, p.y);}
		bool isSet(const int x, const int y) const	{return get(x, y);}

	};

}

#endif // BITMAP_H
