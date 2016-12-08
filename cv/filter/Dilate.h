#ifndef K_CV_DILATE_H
#define K_CV_DILATE_H

#include "../ImageChannel.h"

namespace K {

	namespace CV {

		class Dilate {

		public:

			enum Shape {
				SQUARE_45,
				CIRCLE,
			};

			/**
			 * @brief apply dilation to the given image. all pixels that are similar (threshold) to the given refVal will be "enlarged"
			 * @note erosion can be achieved using dilate with the opposite color value
			 * @param img the image to dilate
			 * @param radius the radius [1:3] to use for "enlarging" found matches
			 * @param refVal the brightness value of the pixels to dilate
			 * @param threshold the threshold (difference from refVal)
			 * @return the dilated image
			 */
			static ImageChannel apply(const ImageChannel& img, const int radius = 1, const Shape shape = Shape::SQUARE_45, const float refVal = 1.0, const float threshold = 0.5) {

				_assertBetween(radius, 1, 3, "invalid radius given");

				// output image
				K::ImageChannel out = img;

				// process each pixel
				for (int y = radius; y < img.getHeight()-radius; ++y) {
					for (int x = radius; x < img.getWidth()-radius; ++x) {

						// get the pixel's value
						const float val = img.get(x, y);

						// perform threshold check. pixel-value too different than reference? -> skip
						const float diff = std::abs(refVal - val);
						if (diff > threshold) {continue;}

						// set the pixel itself
						out.set(x+0, y+0, val);

						// set radius 1 pixels
						if (radius >= 1) {
							out.set(x+1, y+0, val);
							out.set(x-1, y+0, val);
							out.set(x+0, y+1, val);
							out.set(x+0, y-1, val);
						}

						// set radius 2 pixels
						if (radius >= 2) {

							out.set(x+2, y+0, val);
							out.set(x-2, y+0, val);
							out.set(x+0, y+2, val);
							out.set(x+0, y-2, val);

							out.set(x+1, y+1, val);
							out.set(x+1, y-1, val);
							out.set(x-1, y+1, val);
							out.set(x-1, y-1, val);

							if (shape == CIRCLE) {

								out.set(x-2, y-1, val);
								out.set(x-1, y-2, val);

								out.set(x-2, y+1, val);
								out.set(x-1, y+2, val);

								out.set(x+2, y-1, val);
								out.set(x+1, y-2, val);

								out.set(x+2, y+1, val);
								out.set(x+1, y+2, val);

							}

						}

						// set radius 3 pixels
						if (radius >= 3) {

							out.set(x+3, y+0, val);
							out.set(x-3, y+0, val);
							out.set(x+0, y+3, val);
							out.set(x+0, y-3, val);

							out.set(x-2, y-1, val);
							out.set(x-1, y-2, val);

							out.set(x-2, y+1, val);
							out.set(x-1, y+2, val);

							out.set(x+2, y-1, val);
							out.set(x+1, y-2, val);

							out.set(x+2, y+1, val);
							out.set(x+1, y+2, val);

							if (shape == CIRCLE) {
								out.set(x+2, y+2, val);
								out.set(x+2, y-2, val);
								out.set(x-2, y+2, val);
								out.set(x-2, y-2, val);
							}

						}


					}
				}

				// done
				return out;

			}

		};

	}

}

#endif // K_CV_DILATE_H
