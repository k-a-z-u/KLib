#ifndef IMAGEPYRAMID_H
#define IMAGEPYRAMID_H

#include <vector>
#include "ImageChannel.h"
#include "KernelFactory.h"

namespace K {

	class ImagePyramid {

	private:

		/** all layers of the pyramid */
		std::vector<ImageChannel> layers;

	public:

		/** ctor */
		ImagePyramid(ImageChannel& img) {
			layers.push_back(img);
			create();
		}

		/** get the number of layers */
		size_t size() const {
			return layers.size();
		}

		/** get the idx-th layer */
		ImageChannel& get(const int idx) {
			return layers[idx];
		}

	private:

		void create() {

			// start at layer 0
			ImageChannel* prev = &layers[0];

			const Kernel k = KernelFactory::gauss2D(1.0, 5);

			while (true) {

				// size for the current layer
				const int curW = prev->getWidth() / 2;
				const int curH = prev->getHeight() / 2;

				// done?
				if (curW < 1 || curH < 1) {break;}

				// create the image for the next layer
				ImageChannel cur(curW, curH);

				// render
				for (int y = 0; y < curH; ++y) {
					for (int x = 0; x < curW; ++x) {

						float val = 0;	// convolved value
						float sum = 0;	// sum of all used kernel values (for normalization)

						auto lambda = [&] (const int kx, const int ky, const float kv) {

							const int ix = (x*2) + kx;			// translate between cur and previous layer (twice the size) + add window
							const int iy = (y*2) + ky;			// translate between cur and previous layer (twice the size) + add window

							// skip edges
							if (ix < 0 || ix >= prev->getWidth())	{return;}
							if (iy < 0 || iy >= prev->getHeight())	{return;}

							const float sv = prev->get(ix, iy);	// value from the current layer
							val += kv*sv;						// update convolved value
							sum += kv;							// update sum of used kernel values (for normalization)

						};

						// convolve
						k.forEach(lambda);

						// set
						val /= sum;								// normalize
						cur.set(x,y,val);

					}
				}

				// add and proceed
				layers.push_back(cur);
				prev = &layers[layers.size()-1];

			}

		}

	};

}

#endif // IMAGEPYRAMID_H

