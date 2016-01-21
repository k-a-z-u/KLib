#ifndef GAUSS_H
#define GAUSS_H

#include "../ImageChannel.h"
#include "../KernelFactory.h"
#include "../Convolve.h"

namespace K {

	/**
	 * 2D gauss filter using 2x1D gauss
	 */
	class Gauss {

	private:

		Kernel kV;
		Kernel kH;

	public:

		/** ctor */
		Gauss(const float sigma) {
			kH = KernelFactory::gauss1D(sigma);
			kV = KernelFactory::gauss1D(sigma); kV.tilt();
			std::cout << kV << std::endl;
		}

	public:

		/** filter using 2x1D gauss */
		ImageChannel filter(const ImageChannel& src) const {
			return Convolve::run(Convolve::run(src, kH), kV);
		}

	};

}

#endif // GAUSS_H
