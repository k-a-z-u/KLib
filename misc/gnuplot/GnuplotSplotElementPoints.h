#ifndef GNUPLOTSPLOTELEMENTPOINTS_H
#define GNUPLOTSPLOTELEMENTPOINTS_H

#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	class GnuplotSplotElementPoints : public GnuplotSplotElementRaw, public GnuplotAttrColor {

	private:

		float pointSize = 0.2f;
		int pointType = 7;

	public:

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}

		//void setColorHex(const std::string& hex) {this->color = "rgb '" + hex + "'";}

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			ss << "'-' with points ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			if (!color.isAuto()) {ss << " lc " << color.toGP();}
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTPOINTS_H
