#ifndef GNUPLOTPLOTELEMENTHISTOGRAM_H
#define GNUPLOTPLOTELEMENTHISTOGRAM_H



#include <map>
#include "GnuplotPlotElementRaw.h"
#include "../../math/statistics/SimpleHistogram.h"
#include "attributes/GnuplotAttrStroke.h"

namespace K {

	class GnuplotPlotElementHistogram : public GnuplotPlotElementRaw, public GnuplotAttrStroke {


	public:

		void addGeneralTo(std::stringstream& ss) const override {
			ss << "set style fill solid 0.5 border lt -1\n";
		}

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with boxes ";
			ss << attrCustom << " ";
			ss << stroke.toGP();
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add a new sample to the histogram */
		void add(const SimpleHistogram& h) {
			for (const auto& it : h.getFilled()) {
				points.push_back(K::GnuplotPoint2(it.first, it.second));
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

	};

}

#endif // GNUPLOTPLOTELEMENTHISTOGRAM_H
