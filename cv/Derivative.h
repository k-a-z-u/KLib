#ifndef K_CV_DERIVATIVE_H
#define K_CV_DERIVATIVE_H

#include "ImageChannel.h"

namespace K {

	class Derivative {

	public:

		/** derive the image in x direction */
		static ImageChannel getX(const ImageChannel& img) {
			ImageChannel out(img.getWidth(), img.getHeight());
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					out.set(x,y, getX(img,x,y));
				}
			}
			return out;
		}

		/** derive the image in y direction */
		static ImageChannel getY(const ImageChannel& img) {
			ImageChannel out(img.getWidth(), img.getHeight());
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					out.set(x,y, getY(img,x,y));
				}
			}
			return out;
		}

		/** derive the image in xy direction */
		static ImageChannel getXY(const ImageChannel& img) {
			ImageChannel out(img.getWidth(), img.getHeight());
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					out.set(x,y, getXY(img,x,y));
				}
			}
			return out;
		}

		/** get the image's derivative in x direction */
		static float getX(const ImageChannel& img, const int x, const int y) {
			if (x >= img.getWidth()-1) {return 0;}
			return (img.get(x+1,y) - img.get(x,y));
		}

		/** get the image's derivative in y direction */
		static float getY(const ImageChannel& img, const int x, const int y) {
			if (y >= img.getHeight()-1) {return 0;}
			return (img.get(x,y+1) - img.get(x,y));
		}

		/** get the image's derivative in xy direction */
		static float getXY(const ImageChannel& img, const int x, const int y) {
			if (x >= img.getWidth()-1) {return 0;}
			if (y >= img.getHeight()-1) {return 0;}
			return (img.get(x+1,y+1) - img.get(x,y));
		}

	};

}

#endif // K_CV_DERIVATIVE_H

