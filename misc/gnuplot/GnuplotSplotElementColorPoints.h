#ifndef GNUPLOTSPLOTELEMENTCOLORPOINTS_H
#define GNUPLOTSPLOTELEMENTCOLORPOINTS_H

#include <vector>
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

		bool front = false;

	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}

		void setFront(const bool front) {this->front = front;}

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with points palette ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			ss << " title '" << mod->modEntryTitle(title) << "'";
			if (front) {ss << " front ";}
			ss << " ";
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p, const float palette) {
			points.push_back(ColorPoint(p,palette));
		}

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		const std::vector<ColorPoint>& get() const {
			return points;
		}

		bool empty() const override {
			return points.empty();
		}

		void addDataTo(std::ostream& ss) const override {
			for (const ColorPoint& p : points) {
					ss << p.p.x << ' ' << p.p.y << ' ' << p.p.z << ' ' << p.color << "\n";
			}
			ss << "e\n";
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTCOLORPOINTS_H
