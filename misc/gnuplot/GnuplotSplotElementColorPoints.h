#ifndef GNUPLOTSPLOTELEMENTCOLORPOINTS_H
#define GNUPLOTSPLOTELEMENTCOLORPOINTS_H

#include "GnuplotSplotElement.h"

namespace K {

	class GnuplotSplotElementColorPoints : public GnuplotSplotElement {

		struct ColorPoint {
			GnuplotPoint3 p;
			const float color;
			ColorPoint(const GnuplotPoint3& p, const float color) : p(p), color(color) {;}
		};

	private:

		std::vector<ColorPoint> points;

		float pointSize = 0.2;
		int pointType = 7;


	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}


		void addHeaderTo(std::stringstream& ss) const override {
			ss << "'-' with points palette ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			ss << " title '" << title << "'";
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p, const float palette) {
			points.push_back(ColorPoint(p,palette));
		}

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		void addDataTo(std::stringstream& ss) const override {
			for (const ColorPoint& p : points) {
					ss << p.p.x << ' ' << p.p.y << ' ' << p.p.z << ' ' << p.color << "\n";
			}
			ss << "e\n";

		}

	};

}

#endif // GNUPLOTSPLOTELEMENTCOLORPOINTS_H
