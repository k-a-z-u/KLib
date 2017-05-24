#ifndef GNUPLOTPLOTELEMENTCOLORPOINTS_H
#define GNUPLOTPLOTELEMENTCOLORPOINTS_H


#include "GnuplotPlotElement.h"

namespace K {

	class GnuplotPlotElementColorPoints : public GnuplotPlotElement {

		struct ColorPoint {
			GnuplotPoint2 p;
			const float color;
			ColorPoint(const GnuplotPoint2& p, const float color) : p(p), color(color) {;}
		};

	private:

		std::vector<ColorPoint> points;

		float pointSize = 0.2;
		int pointType = 7;


	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}


		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with points palette ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add a new point to output */
		void add(const GnuplotPoint2 p, const float palette) {
			points.push_back(ColorPoint(p,palette));
		}

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		void addDataTo(std::stringstream& ss) const override {
			for (const ColorPoint& p : points) {
					ss << p.p.x << ' ' << p.p.y << ' ' << p.color << "\n";
			}
			ss << "e\n";

		}

	};

}

#endif // GNUPLOTPLOTELEMENTCOLORPOINTS_H
