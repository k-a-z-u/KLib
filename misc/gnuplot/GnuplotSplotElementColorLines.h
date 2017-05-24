#ifndef GNUPLOTSPLOTELEMENTCOLORLINES_H
#define GNUPLOTSPLOTELEMENTCOLORLINES_H


#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"
#include "misc/GnuplotStroke.h"

namespace K {

	class GnuplotSplotElementColorLines : public GnuplotSplotElementRaw, public GnuplotAttrStroke {

		struct ColorPoint {
			GnuplotPoint3 p;
			const float color;
			ColorPoint(const GnuplotPoint3& p, const float color) : p(p), color(color) {;}
		};

		std::vector<ColorPoint> points;

	public:

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			if (empty()) {return;}
			ss << "'-' with lines palette ";
			ss << attrCustom << " ";
			ss << stroke.toGP(false);
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add an empty line */
		void splitFace() {
			GnuplotPoint3 gp = GnuplotPoint3::getEmpty();
			points.push_back(ColorPoint(gp, 0));
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p, const float palette) {
			points.push_back(ColorPoint(p,palette));
		}

		bool empty() const override {
			return points.empty();
		}

		void addDataTo(std::ostream& ss) const override {
			for (const ColorPoint& p : points) {
				if (p.p.isEmpty()) {continue;}
				ss << p.p.x << ' ' << p.p.y << ' ' << p.p.z << ' ' << p.color << "\n";
			}
			ss << "e\n";
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTCOLORLINES_H
