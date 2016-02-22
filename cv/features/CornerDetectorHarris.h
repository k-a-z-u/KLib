#ifndef K_CV_FEATURES_CORNERDETECTORHARRIS_H
#define K_CV_FEATURES_CORNERDETECTORHARRIS_H

#include "Corner.h"
#include "../ImageChannel.h"
#include "../Derivative.h"
#include "../filter/Gauss.h"
#include "../ImageFactory.h"
#include "../LocalMaxima.h"
#include "../segmentation/Segmentation.h"
#include "../filter/Normalize.h"

namespace K {

	class CornerDetectorHarris {

		/** helper-class to calculate the eigenvalues of a 2x2 matrix */
		struct Eigenvalues2 {float e1, e2;};

		/** helper-class to calculate the eigenvalues of a 2x2 matrix */
		struct Matrix2 {

			const float a,b,c,d;

			Matrix2(const float a, const float b, const float c, const float d) : a(a), b(b), c(c), d(d) {;}

			Eigenvalues2 getEigenvalues() const {
				const float T = a+d;
				const float D = a*d - b*c;
				Eigenvalues2 res;
				res.e1 = T/2 + std::sqrt(T*T/4-D);
				res.e2 = T/2 - std::sqrt(T*T/4-D);
				return res;
			}

		};

	private:

		float threshold;
		float sigma;

		LocalMaxima lMax;

	public:



		/** ctor */
		CornerDetectorHarris() : threshold(0.001f), sigma(1.0f), lMax( int(sigma*2), 0.00001f ) {
			;
		}


		/** set the threshold for considering a pixel as a possible corner. default: 0.001f */
		void setThreshold(const float threshold) {
			this->threshold = threshold;
		}

		/** set the sigma used for gaussian blurring of the derivative images. default: 1.0f */
		void setBlurSigma(const float sigma) {
			this->sigma = sigma;
		}

		/** set the size of the neighborhood to examine when searching for local maxima. default: 2*sigma */
		void setNeighborhoodSize(const int size) {
			this->lMax.setSize(size);
		}

		/** find all corners within the provided image-channel */
		std::vector<Corner> getCorners(const ImageChannel& img) {

			// slightly blur the input (remove noise)
			//Gauss g1(0.75);
			//ImageChannel img = g1.filter(src);

			// derive the input image in x and y direction -> Ix and Iy
			ImageChannel imgX = Derivative::getX(img);
			ImageChannel imgY = Derivative::getY(img);

			// calculate Ix * Iy
			ImageChannel imgXY(imgX.getWidth(), imgX.getHeight());
			imgXY.setEach([&] (const int x, const int y) {return imgX.get(x,y) * imgY.get(x,y);});

			// sqaure Ix and Iy (2nd order moments)
			auto lambda = [] (const int x, const int y, const float val) {(void) x; (void) y; return val*val;};
			imgX.forEachModify(lambda);
			imgY.forEachModify(lambda);

			// apply guassian to the derived images (blend possible edges together)
			Gauss g2(sigma);
			imgX = g2.filter(imgX);
			imgY = g2.filter(imgY);
			imgXY = g2.filter(imgXY);

			// calculate R image
			ImageChannel imgR(imgX.getWidth(), imgX.getHeight());
			auto getR = [&] (const int x, const int y) {
				const Matrix2 m(imgX.get(x,y), imgXY.get(x,y), imgXY.get(x,y), imgY.get(x,y));
				const Eigenvalues2 ev = m.getEigenvalues();
				//const float r = ev.e1 - 2.0 * ev.e2;
				//const float r = std::abs(  ev.e1*ev.e2 - k * ((ev.e1+ev.e2)*(ev.e1+ev.e2))  );
				const float r = (ev.e1*ev.e2) / (ev.e1+ev.e2);
				return (r==r && r > threshold) ? (r) : (0);
			};
			imgR.setEach(getR);
			//imgR.normalize();

			// find local maxima within the image
			std::vector<Corner> found;
			ImageChannel imgC(imgX.getWidth(), imgX.getHeight()); imgC.zero();

			lMax.forEach(imgR, [&] (const int x, const int y) {
				found.push_back(Corner(x,y,0));
				imgC.set(x,y,1);
			});

			Normalize::inplace(imgX);
			Normalize::inplace(imgY);
			Normalize::inplace(imgXY);
			Normalize::inplace(imgR);

#ifdef WITH_PNG
			ImageFactory::writePNG("/tmp/i.png", img);
			ImageFactory::writePNG("/tmp/ix.png", imgX);
			ImageFactory::writePNG("/tmp/iy.png", imgY);
			ImageFactory::writePNG("/tmp/ixy.png", imgXY);
			ImageFactory::writePNG("/tmp/ir.png", imgR);
			ImageFactory::writePNG("/tmp/ic.png", imgC);
#endif
			// find corners

			return found;

		}

	};

}

#endif // K_CV_FEATURES_CORNERDETECTORHARRIS_H

