#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "../ImageChannel.h"
#include "../../geo/Point2.h"

#include "../draw/Drawer.h"
#include "../ImageFactory.h"

namespace K {

	struct Ellipse {

	};

	template <typename T> T pow2(T t) {return t*t;}

	struct Group {
		Point2f sum;
		std::vector<Point2f> points;
		Point2f getAvg() const {return sum / (float) points.size();}
		void add(const Point2f p) {sum += p; points.push_back(p);}
		bool contains(Point2f p) const {return getAvg().getDistance(p) < 10;}
	};

	struct Grouper {

		std::vector<Group> groups;

		void add(const Point2f p) {
			for (Group& g : groups) {
				if (g.contains(p)) {g.add(p); return;}
			}
			Group g;
			g.add(p);
			groups.push_back(g);
		}

	};


	/**
	 * src: www.ecse.rpi.edu/homepages/qji/Papers/ellipse_det_icpr02.pdf
	 */
	class Ellipses {

	public:

		/** detect all ellipses within the given binary edge-image */
		std::vector<Ellipse> detect(const ImageChannel& img) {

			std::vector<Point2i> points;

			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					if (img.get(x,y) > 0.4) {points.push_back(Point2i(x,y));}
				}
			}

			const int minDist = 20;

			Grouper grp;
			std::vector<int> acc(1024);


			for (const Point2i p1 : points) {
				for (const Point2i p2 : points) {

					// major-axis half-length
					const float a = float(p1.getDistance(p2)) / 2.0f;

					// needs a certain length of the major-axis
					if (a < minDist) {continue;}
				//	if (p2.x == p1.x) {continue;}
				//	if (p2.y == p1.y) {continue;}

					// ellipse center
					const Point2f p0 = (p1+p2)/2.0f;


					// rotation angle
					const float alpha = std::atan2( float(p2.y - p1.y) , float(p2.x - p1.x) );

					for (const Point2i p3 : points) {

						const float d = p0.getDistance(p3);
						if (d > a) {continue;}			// not a minor axis (larger than the major one)
						if (d < a/4) {continue;}		// ellipse is too narrow

						const float f = p2.getDistance(p3);

						// Law of cosines (get one angle given all 3 sides)
						double cosPhi = (pow2(a) + pow2(d) - pow2(f)) / (2*a*d);
						double cosPhi2 = (pow2(f) - pow2(a) - pow2(d)) / (-2*a*d);
						if (std::abs(cosPhi) > 0.95) {continue;}
						if (std::abs(cosPhi) < 0.05) {continue;}




						const double sinPhi = std::sqrt(1 - pow2(cosPhi));
						//const float phi = std::acos( cosPhi );


						// minor-axis half length
						const float b2 = (pow2(a) * pow2(d) * pow2(sinPhi)) / (pow2(a) - pow2(d) * pow2(cosPhi));

						//if (b2 == b2 && b2 != INFINITY) {
						const float b = std::sqrt(std::abs(b2));

						++acc[(int)b];
						//}

					}


//					auto itMax = std::max_element(acc.begin(), acc.end());
//					const int idx = itMax - acc.begin();
//					if (*itMax > minDist && idx > 1) {
//						std::cout << idx << std::endl;
//					}

					for (int i = 5; i < acc.size(); ++i) {

						// calculate the ellipsi's extend
						const float b = i;
						const float lambda = (a-b)/(a+b);
						const float x = 1+(3*lambda*lambda)/(10+std::sqrt(4-3*lambda*lambda));
						const float u = (a+b)*M_PI*x;

						// needs at-least 25% of the extend to be OK
						if (acc[i] > u*0.25) {
							//std::cout << acc[i] << ":" << u << std::endl;
							grp.add(p0);
						}

					}

					acc.resize(0);
					acc.resize(1024);

				}
			}

			for (Group g : grp.groups) {
				std::cout << "#" << g.getAvg().asString() << " (" << g.points.size() << ")" << std::endl;
			}

			return std::vector<Ellipse>();

		}

	};



}

#endif // ELLIPSE_H
