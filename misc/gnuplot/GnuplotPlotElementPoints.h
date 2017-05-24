#ifndef GNUPLOTPLOTELEMENTPOINTS_H
#define GNUPLOTPLOTELEMENTPOINTS_H

#include <vector>

#include "GnuplotPlotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	class GnuplotPlotElementPoints : public GnuplotPlotElementRaw, public GnuplotAttrColor {

	private:

		float pointSize = 0.2f;
		int pointType = 7;

	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with points ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			if (!color.isAuto()) {ss << " lc " << color.toGP();}
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add a new point to output */
		void add(const GnuplotPoint2 p) {
			points.push_back(p);
		}

		/** remove all points */
		void clear() {
			points.clear();
		}

	};

}

#endif // GNUPLOTPLOTELEMENTPOINTS_H
