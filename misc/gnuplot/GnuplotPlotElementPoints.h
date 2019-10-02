#ifndef GNUPLOTPLOTELEMENTPOINTS_H
#define GNUPLOTPLOTELEMENTPOINTS_H

#include <vector>

#include "GnuplotPlotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"
#include <functional>

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

		/** remove all points if the given function matches */
		void removeIf(std::function<bool(GnuplotPoint2)> func) {
			for (size_t i = 0; i < points.size(); ++i) {
				const bool remove = func(points[i]);
				if (remove) {points.erase(points.begin()+i); --i;}
			}
		}

		void keepLatest(int cnt) {
			int toRemove = points.size() - cnt;
			if (toRemove > 0) {
				points.erase(points.begin(), points.begin() + toRemove);
			}
		}

	};

}

#endif // GNUPLOTPLOTELEMENTPOINTS_H
