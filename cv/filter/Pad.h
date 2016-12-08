#ifndef K_CV_PAD_H
#define K_CV_PAD_H

#include "../ImageChannel.h"
#include "../draw/Drawer.h"

namespace K {

	namespace CV {

		class Pad {

		public:


			/** resize and pad with black pixels */
			static K::ImageChannel center(const ImageChannel& img, const int w, const int h) {

				K::ImageChannel out(w,h);
				Drawer drawer(out);

				const int dx = w/2 - img.getWidth()/2;
				const int dy = h/2 - img.getHeight()/2;

				drawer.drawImage( img, Point2i(dx, dy) );

				return out;

			}


		};

	}


}

#endif // K_CV_PAD_H
