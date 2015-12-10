#ifndef K_CV_LOCALMAXIMA_H
#define K_CV_LOCALMAXIMA_H

#include "ImageChannel.h"

namespace K {

	class LocalMaxima {

	private:

		/** the neighborhood-size to search for other maxima */
		int size;

		/** the minimum value to allow for local-maxima-checking */
		float threshold;

	public:

		/** ctor */
		LocalMaxima(const int size = 1, const float threshold = 0.00001f) : size(size), threshold(threshold) {
			;
		}

		void forEach(const ImageChannel& img, std::function<void(const int x, const int y)> func) {
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					if (isMaxima(img,x,y)) {func(x,y);}
				}
			}
		}

		/** is the given point above the threshold and also a local maxima? */
		bool isMaxima(const ImageChannel& img, const int cx, const int cy) const {

			// value of the point at (cx,cy)
			const float cv = img.get(cx,cy);

			// ignore values below the threshold
			if (cv < threshold) {return false;}

			// to-be-checked region
			const int x1 = (cx-size > 0) ? (cx-size) : (0);
			const int y1 = (cy-size > 0) ? (cy-size) : (0);
			const int x2 = (cx+size < img.getWidth())	? (cx+size) : (img.getWidth()-1);
			const int y2 = (cy+size < img.getHeight())	? (cy+size) : (img.getHeight()-1);

			// check neighborhood
			for (int y = y1; y <= y2; ++y) {
				for (int x = x1; x <= x2; ++x) {
					if (x == cx && y == cy)	{continue;}			// skip the center itself
					if (img.get(x,y) > cv)	{return false;}		// other maxima
				}
			}

			// is maxima!
			return true;

		}

	};

}

#endif // K_CV_LOCALMAXIMA_H

