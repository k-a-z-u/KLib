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
			float avg1Value;
			float avg2Value;
			float avg1GradX;
			float avg1GradY;
			float avg1Sigma;
			float sigma;
			float diff(const FeatureVec& o) const {
				return
					K::NormalDistribution::getProbability(o.avg2Value, 0.1, avg2Value) *
					K::NormalDistribution::getProbability(o.avg1Value, 0.1, avg1Value) *
					//K::NormalDistribution::getProbability(o.avg1GradX, 0.1, avg1GradX) *	// blurred -> tiny sigma
					//K::NormalDistribution::getProbability(o.avg1GradY, 0.1, avg1GradY) *	// blurred -> tiny sigma
					K::NormalDistribution::getProbability(o.avg1Sigma, 0.1, avg1Sigma) *	// blurred -> tiny sigma
					K::NormalDistribution::getProbability(o.sigma, 0.1 , sigma) *			// unblurred -> larger sigma
					1.0f;
			}
		};

	public:

		static DataMatrix<FeatureVec> getFeatures(const ImageChannel& img, const int win = 9) {

			CV::Gauss g1(1.5f);
			CV::Gauss g2(win/2);

			ImageChannel imgBlur1 = g1.filter(img);
			ImageChannel imgBlur2 = g2.filter(img);

			// get new images for both, x and y gradient (1st derivative)
			ImageChannel imgBlur2X = Derivative::getX(imgBlur2);
			ImageChannel imgBlur2Y = Derivative::getY(imgBlur2);

			ImageChannel imgBlur1X = Derivative::getX(imgBlur1);
			ImageChannel imgBlur1Y = Derivative::getY(imgBlur1);


//			Normalize::inplace(imgX);
//			Normalize::inplace(imgY);
			ImageFactory::writeJPEG("/tmp/blur1.jpg", imgBlur1);
			ImageFactory::writeJPEG("/tmp/blur2.jpg", imgBlur2);
			ImageFactory::writeJPEG("/tmp/imgX.jpg", imgBlur1X);
			ImageFactory::writeJPEG("/tmp/imgY.jpg", imgBlur1Y);

			const int s = win/2;
			DataMatrix<FeatureVec> dm(img.getWidth(), img.getHeight());

			// process each pixel
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {

					float cnt = ((s*2)+1) * ((s*2)+1);
					float avg1Sum = 0;
					float avg1Sum2 = 0;
					float avg2Sum = 0;
					float avg2Sum2 = 0;
					float sum = 0;
					float sum2 = 0;


					// process the window
					for (int y1 = -s; y1 <= +s; ++y1) {
						for (int x1 = -s; x1 <= +s; ++x1) {

							float val =		img.getClamped(x+x1, y+y1);
							float avg1Val =	imgBlur1.getClamped(x+x1, y+y1);
							float avg2Val =	imgBlur2.getClamped(x+x1, y+y1);

							sum += val;
							sum2 += val*val;

							avg1Sum += avg1Val;
							avg1Sum2 += avg1Val*avg1Val;

							avg2Sum += avg2Val;
							avg2Sum2 += avg2Val*avg2Val;

						}
					}


					FeatureVec vec;
					vec.avg2Value =	imgBlur2.getClamped(x,y);
					vec.avg1Value = imgBlur1.getClamped(x,y);
					vec.avg1GradX =	imgBlur1X.getClamped(x, y);
					vec.avg1GradY =	imgBlur1Y.getClamped(x, y);
					vec.avg1Sigma =	std::sqrt(avg1Sum2/cnt -	((avg1Sum/cnt)*(avg1Sum/cnt)));
					vec.sigma =		std::sqrt(sum2/cnt -		((sum/cnt)*(sum/cnt)));
					dm.set(x,y,vec);

				}
			}

			return dm;

		}



	};

}

#endif // SIMPLE_H
