#ifndef GNUPLOTSPLOTELEMENTLINES_H
#define GNUPLOTSPLOTELEMENTLINES_H

#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	class GnuplotSplotElementLines : public GnuplotSplotElementRaw, public GnuplotAttrColor {

	private:

		int lineWidth = 1;

	public:

		/** set the line-width to use for drawing */
		void setLineWidth(const int lineWidth) {this->lineWidth = lineWidth;}

		void addHeaderTo(std::ostream& ss) const override {
			if (empty()) {return;}
			ss << "'-' with lines ";
			ss << attrCustom << " ";
			ss << " lw " << lineWidth;
			ss << " lc " << color;
			ss << " title '" << title << "'";
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
