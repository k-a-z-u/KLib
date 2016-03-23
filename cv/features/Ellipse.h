#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "../ImageChannel.h"
#include "../Point2.h"

namespace K {

	struct Ellipse {

	};

	template <typename T> T pow2(T t) {return t*t;}

	class Ellipses {

	public:

		/** detect all ellipses within the given binary edge-image */
		std::vector<Ellipse> detect(const ImageChannel& img) {

			std::vector<Point2i> points;

			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					if (img.get(x,y) > 0.5) {points.push_back(Point2i(x,y));}
				}
			}

			const int minDist = 20;

			std::vector<int> acc(1024);

			for (Point2i p1 : points) {
				for (Point2i p2 : points) {

					if (p2.getDistance(p1) < minDist) {continue;}
					if (p2.x == p1.x) {continue;}
					if (p2.y == p1.y) {continue;}

					// ellipse center
					const Point2f p0( float(p1.x + p2.x) / 2.0f, float(p1.y + p2.y) / 2.0f );

					// major-axis half-length
					const float a = p1.getDistance(p2) / 2.0f;//std::sqrt( float( pow2(p2.x - p1.x) + pow2(p2.y - p1.y) ) ) / 2.0f;

					// rotation angle
					const float alpha = std::atan( float(p2.y - p1.y) / float(p2.x - p1.x) );

					for (Point2i p3 : points) {

						const float d = p0.getDistance(p3);
						if (d < minDist) {continue;}
						const float f = p2.getDistance(p3);

						const float cosPhi = (pow2(a) + pow2(d) - pow2(f)) / (2*a*d);
						const float phi = std::acos( cosPhi );

						// minor-axis half length
						const float b2 = (pow2(a) * pow2(d) * pow2(sin(phi))) / (pow2(a) * -pow2(d) * pow2(cos(phi)));

						++acc[(int)b2];

					}


					auto itMax = std::max_element(acc.begin(), acc.end());
					const int idx = itMax - acc.begin();
					if (*itMax > minDist) {
						std::cout << idx << std::endl;
					}

				}
			}


			return std::vector<Ellipse>();

		}

	};

}

#endif // ELLIPSE_H
