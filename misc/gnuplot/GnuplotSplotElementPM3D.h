#ifndef GNUPLOTSPLOTELEMENTPM3D_H
#define GNUPLOTSPLOTELEMENTPM3D_H


#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"
#include "misc/GnuplotStroke.h"

namespace K {

	class GnuplotSplotElementPM3D : public GnuplotSplotElementRaw, public GnuplotAttrStroke {

	public:

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			if (empty()) {return;}
			ss << "'-' with pm3d ";
			ss << attrCustom << " ";
			ss << stroke.toGP();
			//ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		/** add an empty line */
		void splitFace() {
			points.push_back(GnuplotPoint3::getEmpty());
		}

		/** add an unnconected segment from A to B */
		void addPoly(const std::vector<GnuplotPoint3>& pts, const bool close) {
			for (const GnuplotPoint3& pt : pts) {
				points.push_back(pt);
			}
			if (close) {points.push_back(pts.front());}
			points.push_back(GnuplotPoint3::getEmpty());
			//points.push_back(GnuplotPoint3::getEmpty());
		}


	};

}


#endif // GNUPLOTSPLOTELEMENTPM3D_H
