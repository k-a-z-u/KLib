#ifndef GNUPLOTSPLOTELEMENTHEATMAP_H
#define GNUPLOTSPLOTELEMENTHEATMAP_H

#include "GnuplotSplotElementRaw.h"

namespace K {

	class GnuplotSplotElementHeatMap : public GnuplotSplotElementRaw {

	private:

	public:

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with image";
			ss << " title '" << mod->modEntryTitle(title) << "' ";
			ss << attrCustom;
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p) {
			points.push_back(p);
		}

		void addRow() {
			points.push_back(GnuplotPoint3::getEmpty());
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTHEATMAP_H
