#ifndef GNUPLOTSPLOTELEMENTLINES_H
#define GNUPLOTSPLOTELEMENTLINES_H

#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"
#include "misc/GnuplotStroke.h"

namespace K {

	class GnuplotSplotElementLines : public GnuplotSplotElementRaw, public GnuplotAttrStroke {

	public:

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			if (empty()) {return;}
			ss << "'-' with lines ";
			ss << attrCustom << " ";
			ss << stroke.toGP();
			//ss << " lw " << lineWidth;
			//if (!color.isAuto()) {ss << " lc " << color.toGP();}
			//if (dashType != 1) {ss << " dashtype " << dashType;}
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add an empty line */
		void splitFace() {
			points.push_back(GnuplotPoint3::getEmpty());
		}

		/** add an unnconected segment from A to B */
		void addSegment(const GnuplotPoint3 from, const GnuplotPoint3 to) {
			points.push_back(from);
			points.push_back(to);
			points.push_back(GnuplotPoint3::getEmpty());
			points.push_back(GnuplotPoint3::getEmpty());
		}


	};

}

#endif // GNUPLOTSPLOTELEMENTLINES_H
