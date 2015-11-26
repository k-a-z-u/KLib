#ifndef K_CV_LOCALMAXIMA_H
#define K_CV_LOCALMAXIMA_H

#include "ImageChannel.h"

namespace K {

	class LocalMaxima {

	private:

		/** the neighborhood-size to search for other maxima */
		int size;

	public:

		/** ctor */
		LocalMaxima(const int size = 1) : size(size) {
			;
		}

		void forEach(const ImageChannel& img, std::function<void(const int x, const int y)> func) {
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					if (isMaxima(img,x,y)) {func(x,y);}
				}
			}
		}

		/** is the given point a local maxima? */
		bool isMaxima(const ImageChannel& img, const int cx, const int cy) const {

			const int x1 = (cx-size >= 0) ? (cx-size) : (0);
			const int y1 = (cy-size >= 0) ? (cy-size) : (0);

			const int x2 = (cx+size < img.getWidth()) ? (cx+size) : (img.getWidth()-1);
			const int y2 = (cy+size < img.getHeight()) ? (cy+size) : (img.getHeight()-1);

			const float cv = img.get(cx,cy);

			// check neighborhood
			for (int y = y1; y <= y2; ++y) {
				for (int x = x1; x <= x2; ++x) {
					if (x == cx && y == cy) {continue;}			// skip the center itself
					if (img.get(x,y) >= cv) {return false;}		// other maxima
				}
			}

			// is maxima!
			return true;

		}

	};

}

#endif // K_CV_LOCALMAXIMA_H

