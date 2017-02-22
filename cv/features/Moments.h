#ifndef MOMENTS_H
#define MOMENTS_H

#include "../ImageChannel.h"
#include "../../geo/Point2.h"
#include "../../geo/Size2.h"

namespace K {

	class Moments {

	public:

		template <int p, int q> static float get(const ImageChannel& img, const Point2i center, const Size2i window) {

			const int x1 = - halfL(window.w);
			const int x2 = + halfH(window.w);

			const int y1 = - halfL(window.h);
			const int y2 = + halfH(window.h);

			float sum = 0;

			for (int oy = y1; oy < y2; ++oy) {
				for (int ox = x1; ox < x2; ++ox) {

					const int x = center.x + ox;
					const int y = center.y + oy;

					if (x < 0)					{continue;}
					if (y < 0)					{continue;}
					if (x >= img.getWidth())	{continue;}
					if (y >= img.getHeight())	{continue;}

					const float mx = std::pow(ox+x1, p);
					const float my = std::pow(oy+y1, q);

					sum += mx * my * img.get(x, y);

				}
			}

			return sum;

		}

	private:

		static inline int halfL(const int inp) { return inp/2; }
		static inline int halfH(const int inp) { return (inp+1)/2; }

	};

}

#endif // MOMENTS_H
