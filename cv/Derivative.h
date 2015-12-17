#ifndef K_CV_DERIVATIVE_H
#define K_CV_DERIVATIVE_H

#include "ImageChannel.h"
#include "../Assertions.h"

namespace K {

	class Derivative {

	public:

		/** derive the image in x direction */
		static ImageChannel getX(const ImageChannel& img) {
			ImageChannel out(img.getWidth(), img.getHeight());
			out.zero();
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth() - 1; ++x) {
					out.set(x,y, getX(img,x,y));
				}
			}
			return out;
		}

		/** derive the image in y direction */
		static ImageChannel getY(const ImageChannel& img) {
			ImageChannel out(img.getWidth(), img.getHeight());
			out.zero();
			for (int y = 0; y < img.getHeight() - 1; ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					out.set(x,y, getY(img,x,y));
				}
			}
			return out;
		}

//		/** derive the image in xy direction */
//		static ImageChannel getXY(const ImageChannel& img) {
//			ImageChannel out(img.getWidth(), img.getHeight());
//			for (int y = 0; y < img.getHeight(); ++y) {
//				for (int x = 0; x < img.getWidth(); ++x) {
//					out.set(x,y, getXY(img,x,y));
//				}
//			}
//			return out;
//		}

		/** get the image's derivative in x direction (x,x+1) */
		static float getX(const ImageChannel& img, const int x, const int y) {
			_assertBetween(x, 0, img.getWidth() - 2, "x-index out of bounds");
			_assertBetween(y, 0, img.getHeight() - 1, "y-index out of bounds");
			if (x >= img.getWidth() - 1) {return 0;}
			return (img.get(x+1,y) - img.get(x,y));
		}

		/** get the image's derivative in x direction (x-1,x+1) */
		static float getXcen(const ImageChannel& img, const int x, const int y) {
			_assertBetween(x, 1, img.getWidth() - 2, "x-index out of bounds");
			_assertBetween(y, 0, img.getHeight() - 1, "y-index out of bounds");
			if (x >= img.getWidth() - 1) {return 0;}
			return (img.get(x+1,y) - img.get(x-1,y));
		}


		/** get the image's derivative in y direction (y, y+1) */
		static float getY(const ImageChannel& img, const int x, const int y) {
			_assertBetween(x, 0, img.getWidth() - 1, "x-index out of bounds");
			_assertBetween(y, 0, img.getHeight() - 2, "y-index out of bounds");
			return (img.get(x,y+1) - img.get(x,y));
		}

		/** get the image's derivative in y direction (y-1, y+1) */
		static float getYcen(const ImageChannel& img, const int x, const int y) {
			_assertBetween(x, 0, img.getWidth() - 1, "x-index out of bounds");
			_assertBetween(y, 1, img.getHeight() - 2, "y-index out of bounds");
			return (img.get(x,y+1) - img.get(x,y-1));
		}


//		/** get the image's derivative in xy direction (x|y, x+1|y+1) */
//		static float getXY(const ImageChannel& img, const int x, const int y) {
//			if (x >= img.getWidth()-1) {return 0;}
//			if (y >= img.getHeight()-1) {return 0;}
//			return (img.get(x+1,y+1) - img.get(x,y));
//		}

	};

}

#endif // K_CV_DERIVATIVE_H

