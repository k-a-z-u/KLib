#ifndef K_CV_FEATURES_CORNERDETECTORHARRIS_H
#define K_CV_FEATURES_CORNERDETECTORHARRIS_H

#include "Corner.h"
#include "../ImageChannel.h"
#include "../Derivative.h"
#include "../KernelFactory.h"
#include "../Convolve.h"
#include "../ImageFactory.h"
#include "../LocalMaxima.h"

namespace K {

	class CornerDetectorHarris {

		struct Matrix2 {

			float a,b,c,d;
			struct Eigenvalues {float e1, e2;};

			Matrix2() : a(0), b(0), c(0), d(0) {;}
			Matrix2(const float a, const float b, const float c, const float d) : a(a), b(b), c(c), d(d) {;}

			Eigenvalues getEigenvalues() const {
				const float T = a+d;
				const float D = a*d - b*c;
				Eigenvalues res;
				res.e1 = T/2 + std::sqrt(T*T/4-D);
				res.e2 = T/2 - std::sqrt(T*T/4-D);
				return res;
			}

		};

	private:

		float k;
		float threshold;
		float sigma;

	public:



		/** ctor */
		CornerDetectorHarris(const float k = 0.02f, const float threshold = 0.001f, const float sigma = 1.0f) :
			k(k), threshold(threshold), sigma(sigma) {
			;
		}


		/** find all corners within the provided image-channel */
		std::vector<Corner> getCorners(const ImageChannel& _img) {

			Kernel kernel = KernelFactory::gauss2D(1, 5);

			const ImageChannel img = Convolve::run(_img, kernel);

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

			// apply guassian to the derived images

			imgX = Convolve::run(imgX, kernel);
			imgY = Convolve::run(imgY, kernel);
			imgXY = Convolve::run(imgXY, kernel);

			// calculate R image
			ImageChannel imgR(imgX.getWidth(), imgX.getHeight());
			auto getR = [&] (const int x, const int y) {
				const Matrix2 m(imgX.get(x,y), imgXY.get(x,y), imgXY.get(x,y), imgY.get(x,y));
				const Matrix2::Eigenvalues ev = m.getEigenvalues();
				//const float r = ev.e1 - 2.0 * ev.e2;
				//const float r = std::abs(  ev.e1*ev.e2 - k * ((ev.e1+ev.e2)*(ev.e1+ev.e2))  );
				const float r = (ev.e1*ev.e2) / (ev.e1+ev.e2);
				return (r==r && r > threshold) ? (r) : (0);//(r > 0.005) ? (r) : (0);
			};
			imgR.setEach(getR);

			// find local maxima within the image
			LocalMaxima lMax(2);
			std::vector<Corner> found;
			ImageChannel imgC(imgX.getWidth(), imgX.getHeight()); imgC.zero();
			const int s = 1;
			lMax.forEach(img, [&] (const int x, const int y) {
				found.push_back(Corner(x,y,0));
				imgC.set(x,y,1);
			});

			imgX.normalize();;
			imgY.normalize();;
			imgXY.normalize();
			imgR.normalize();
			ImageFactory::writePNG("/tmp/ix.png", imgX);
			ImageFactory::writePNG("/tmp/iy.png", imgY);
			ImageFactory::writePNG("/tmp/ixy.png", imgXY);
			ImageFactory::writePNG("/tmp/ir.png", imgR);
			ImageFactory::writePNG("/tmp/ic.png", imgC);
			// find corners








			return found;

		}

	};

}

#endif // K_CV_FEATURES_CORNERDETECTORHARRIS_H

