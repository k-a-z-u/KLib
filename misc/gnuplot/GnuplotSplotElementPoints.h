#ifndef GNUPLOTSPLOTELEMENTPOINTS_H
#define GNUPLOTSPLOTELEMENTPOINTS_H

#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	class GnuplotSplotElementPoints : public GnuplotSplotElementRaw, public GnuplotAttrColor {

	private:

		float pointSize = 0.2f;
		int pointType = 7;
		std::string file = "-";		// no-file. use manual data

	public:

		void setFile(const std::string& file) {this->file = file;}

		void setPointType(const int t) {this->pointType = t;}

		void setPointSize(const float s) {this->pointSize = s;}

		bool empty() const override {
			return (file == "-") && (points.empty());					// when an external file is given: assume non-empty
		}

		void addDataTo(std::ostream& ss) const override {
			if (file == "-") {GnuplotSplotElementRaw::addDataTo(ss);}	// only in case of non-external-file
		}

		//void setColorHex(const std::string& hex) {this->color = "rgb '" + hex + "'";}

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			ss << "'" << file << "' with points ";
			ss << attrCustom << " ";
			ss << " pt " << pointType;
			ss << " ps " << pointSize;
			if (!color.isAuto()) {ss << " lc " << color.toGP();}
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTPOINTS_H
