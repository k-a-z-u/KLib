#ifndef CLEAN_H
#define CLEAN_H

#include "../ImageChannel.h"

namespace K {

	namespace CV {

		class Clean {

		public:

			/**
			 * @brief filter black/white images by calculating the average within a window and setting the output depending
			 * on whether the average is above or below the given threshold
			 * @param img the black/white image to clear
			 * @param radius half of the rectangular window size
			 * @param threshold the threshold to use for the average
			 * @return the filtered image
			 */
			static K::ImageChannel avgThreshold(const K::ImageChannel& img, const int radius, const float threshold) {

				K::ImageChannel out(img.getWidth(), img.getHeight());

				const int cnt = ((2*radius)+1) * ((2*radius)+1);

				for (int y = radius; y < img.getHeight()-radius; ++y) {
					for (int x = radius; x < img.getWidth()-radius; ++x) {

						const float val = img.get(x,y);
						if (val < threshold) {out.set(x,y,0.0f); continue;}

						float sum = 0;
						for (int y1 = y-radius; y1 <= y+radius; ++y1) {
							for (int x1 = x-radius; x1 <= x+radius; ++x1) {
								sum += img.get(x1, y1);
							}
						}

						const float avg = sum / (float)cnt;
						const float res = (avg > threshold) ? (1.0f) : (0.0f);
						out.set(x,y,res);

					}
				}

				return out;

			}

		};

	}

}

#endif // CLEAN_H
