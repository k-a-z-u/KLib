#ifndef GNUPLOTPLOTELEMENTPOINTS_H
#define GNUPLOTPLOTELEMENTPOINTS_H


#include "GnuplotPlotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	class GnuplotPlotElementPoints : public GnuplotPlotElementRaw {

	private:

		float pointSize = 0.2f;
		int pointType = 7;


	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}

		void setColorHex(const std::string& hex) {this->color = "rgb '" + hex + "'";}

		void addHeaderTo(std::stringstream& ss) const override {
			ss << "'-' with points ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			ss << " lc " << color;
			ss << " title '" << title << "'";
		}

		/** add a new point to output */
		void add(const GnuplotPoint2 p) {
			points.push_back(p);
		}

	};

}

#endif // GNUPLOTPLOTELEMENTPOINTS_H
