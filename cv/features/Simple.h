#ifndef SIMPLE_H
#define SIMPLE_H

#include "../DataMatrix.h"
#include "../ImageChannel.h"
#include "../Derivative.h"
#include "../filter/Gauss.h"
#include "../filter/Normalize.h"

#include "../ImageFactory.h"

#include "../../math/distribution/Normal.h"

namespace K {

	class SimpleFeatures {

	public:

		struct FeatureVec {
			float avgValue;
			float avgGradX;
			float avgGradY;
			float avgSigma;
			float sigma;
			float diff(const FeatureVec& o) const {
				return
					K::NormalDistribution::getProbability(o.avgValue, 0.05, avgValue) *
					K::NormalDistribution::getProbability(o.avgGradX, 0.1, avgGradX) *	// blurred -> tiny sigma
					K::NormalDistribution::getProbability(o.avgGradY, 0.1, avgGradY) *	// blurred -> tiny sigma
					K::NormalDistribution::getProbability(o.avgSigma, 0.1, avgSigma) *	// blurred -> tiny sigma
					K::NormalDistribution::getProbability(o.sigma, 0.1 , sigma) *			// unblurred -> larger sigma
					1.0f;
			}
		};

	public:

		static DataMatrix<FeatureVec> getFeatures(const ImageChannel& img, const int win = 9) {

			Gauss g1(1.0f);
			Gauss g2(10.0f);
			ImageChannel imgBlur1 = g1.filter(img);
			ImageChannel imgBlur2 = g2.filter(img);

			// get new images for both, x and y gradient (1st derivative)
			ImageChannel imgX = Derivative::getX(imgBlur2);
			ImageChannel imgY = Derivative::getY(imgBlur2);


//			Normalize::inplace(imgX);
//			Normalize::inplace(imgY);
			ImageFactory::writeJPEG("/tmp/blur1.jpg", imgBlur1);
			ImageFactory::writeJPEG("/tmp/blur2.jpg", imgBlur2);
			ImageFactory::writeJPEG("/tmp/imgX.jpg", imgX);
			ImageFactory::writeJPEG("/tmp/imgY.jpg", imgY);

			const int s = win/2;
			DataMatrix<FeatureVec> dm(img.getWidth(), img.getHeight());

			// process each pixel
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {

					float cnt = ((s*2)+1) * ((s*2)+1);
					float avgSum = 0;
					float avgSum2 = 0;
					float sum = 0;
					float sum2 = 0;


					// process the window
					for (int y1 = -s; y1 <= +s; ++y1) {
						for (int x1 = -s; x1 <= +s; ++x1) {

							float val =		imgBlur1.getClamped(x+x1, y+y1);
							float avgVal =	imgBlur2.getClamped(x+x1, y+y1);

							avgSum += avgVal;
							avgSum2 += avgVal*avgVal;

							sum += val;
							sum2 += val*val;

						}
					}


					FeatureVec vec;
					vec.avgValue =	imgBlur2.getClamped(x, y);
					vec.avgGradX =	imgX.getClamped(x, y);
					vec.avgGradY =	imgY.getClamped(x, y);
					vec.avgSigma =	std::sqrt(avgSum2/cnt -	((avgSum/cnt)*(avgSum/cnt)));
					vec.sigma =		std::sqrt(sum2/cnt -		((sum/cnt)*(sum/cnt)));
					dm.set(x,y,vec);

				}
			}

			return dm;

		}



	};

}

#endif // SIMPLE_H
