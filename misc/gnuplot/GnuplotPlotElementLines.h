#ifndef GNUPLOTPLOTELEMENTLINES_H
#define GNUPLOTPLOTELEMENTLINES_H

#include <functional>
#include <vector>
#include "GnuplotPlotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"
#include <functional>

namespace K {

	class GnuplotPlotElementLines : public GnuplotPlotElementRaw, public GnuplotAttrStroke {

	public:

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {

			ss << "'-' with lines ";
			ss << attrCustom << " ";
			ss << stroke.toGP();
			ss << " title '" << mod->modEntryTitle(title) << "' ";
			ss << getUseAxis();

		}

		/** add a new point to output */
		void add(const GnuplotPoint2 p) {
			points.push_back(p);
		}

		/** remove all points if the given function matches */
		void removeIf(std::function<bool(GnuplotPoint2)> func) {
			for (size_t i = 0; i < points.size(); ++i) {
				const bool remove = func(points[i]);
				if (remove) {points.erase(points.begin()+i); --i;}
			}
		}

		/** remove all points */
		void clear() {
			points.clear();
		}

		/** remove the given index */
		void remove(const int idx) {
			points.erase(points.begin() + idx);
		}

		/** number of entries */
		size_t size() const {
			return points.size();
		}

		/** add an unnconected segment from A to B */
		void addSegment(const GnuplotPoint2 from, const GnuplotPoint2 to) {
			points.push_back(from);
			points.push_back(to);
			points.push_back(GnuplotPoint2::getEmpty());
			points.push_back(GnuplotPoint2::getEmpty());
		}


	};

}


#endif // GNUPLOTPLOTELEMENTLINES_H
