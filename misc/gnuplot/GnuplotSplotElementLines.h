#ifndef GNUPLOTSPLOTELEMENTLINES_H
#define GNUPLOTSPLOTELEMENTLINES_H

#include <vector>
#include "GnuplotSplotElementRaw.h"

namespace K {

	class GnuplotSplotElementLines : public GnuplotSplotElementRaw {

	private:

		int lineWidth = 1;

	public:

		/** set the line-width to use for drawing */
		void setLineWidth(const int lineWidth) {this->lineWidth = lineWidth;}



		void addHeaderTo(std::stringstream& ss) const override {
			ss << "'-' with lines ";
			ss << " lw " << lineWidth;
			ss << " lc " << color;
			ss << " title '" << title << "'";
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p) {
			points.push_back(p);
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
