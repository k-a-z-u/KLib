#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include "ImageChannel.h"
#include "../geo/Point2.h"

namespace K {

	namespace CV {

		class ImageAnalyzer {

		private:

			const K::ImageChannel& img;


		public:

			struct Stats {
				float min = +999999;
				float max = -999999;
				float avg;
				float stdDev;
			} global;

			struct Extrema {
				std::vector<K::Point2i> minima;
				std::vector<K::Point2i> maxima;
			};

		public:

			/** ctor with the image */
			ImageAnalyzer(const K::ImageChannel& img) : img(img) {
				buildGlobal();
			}

			/** get all global stats for the image */
			const Stats& getGlobalStats() const {
				return global;
			}

			/** rebuild, e.g. when the image was changed */
			void rebuild() {
				buildGlobal();
			}

			/** get all points that are local extrema */
			Extrema getLocalExtrema( const int winSize, const float threshold = 0.05f ) {

				Extrema res;

				for (int y = 0; y < img.getHeight(); ++y) {
					for (int x = 0; x < img.getWidth(); ++x) {

						const float val = img.get(x,y);
						int numGreater = 0;
						int numSmaller = 0;
						int numTotal = 0;

						// check neighbors
						for (int oy = -winSize; oy <= +winSize; ++oy) {
							for (int ox = -winSize; ox <= +winSize; ++ox) {

								const int x1 = x + ox;
								const int y1 = y + oy;

								if (!img.contains(x1,y1)) {continue;}
								const float val2 = img.get(x1,y1);

								if (val2 > val) {++numGreater;}
								if (val2 < val) {++numSmaller;}
								++numTotal;

							}
						}

						// minima?
						if (numGreater < numTotal*threshold) {res.maxima.push_back(K::Point2i(x,y));}
						if (numSmaller < numTotal*threshold) {res.minima.push_back(K::Point2i(x,y));}

					}
				}

				return res;

			}

		private:

			/** get global image facts */
			void buildGlobal() {

				float sum = 0;
				float sum2 = 0;

				for (size_t i = 0; i < img.getSizeElements(); ++i) {
					const float val = img.getData()[i];
					sum += val;
					sum2 += val*val;
					if (val < global.min) {global.min = val;}
					if (val > global.max) {global.max = val;}
				}

				const float avg = sum/img.getSizeElements();
				const float avg2 = sum2/img.getSizeElements();
				global.avg = avg;
				global.stdDev = std::sqrt( avg2 - (avg*avg) );

			}


		};

	}

}


#endif // IMAGEANALYZER_H
