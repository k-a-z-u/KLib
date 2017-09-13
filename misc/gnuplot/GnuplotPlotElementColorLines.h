#ifndef GNUPLOTPLOTELEMENTCOLORLINES_H
#define GNUPLOTPLOTELEMENTCOLORLINES_H

#include <vector>
#include "GnuplotPlotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"

namespace K {

	class GnuplotPlotElementColorLines : public GnuplotPlotElementRaw, public GnuplotAttrStroke {

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

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with lines palette ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add an empty line */
		void splitFace() {
			GnuplotPoint2 gp = GnuplotPoint2::getEmpty();
			points.push_back(ColorPoint(gp, 0));
		}

		/** add a new point to output */
		void add(const GnuplotPoint2 p, const float palette) {
			points.push_back(ColorPoint(p,palette));
		}

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		bool isEmpty() const override {
			return points.empty();
		}

		void addDataTo(std::stringstream& ss) const override {
			for (const ColorPoint& p : points) {
					ss << p.p.x << ' ' << p.p.y << ' ' << p.color << "\n";
			}
			ss << "e\n";

		}

	};

}


#endif // GNUPLOTPLOTELEMENTCOLORLINES_H
