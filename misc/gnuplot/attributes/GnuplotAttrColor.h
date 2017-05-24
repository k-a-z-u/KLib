#ifndef GNUPLOTATTRCOLOR_H
#define GNUPLOTATTRCOLOR_H

#include "../misc/GnuplotColor.h"

namespace K {

	class GnuplotAttrColor {

	protected:

		GnuplotColor color;

	public:

		/** set the color to use for drawing this element using a hex-value */
		//void setColorHex(const std::string& hex) {this->color = GnuplotColor::fromHexStr(hex);}

		/** set the color to use */
		void setColor(const GnuplotColor& c) {
			this->color = c;
		}

		/** get the color in use */
		GnuplotColor& getColor() {
			return this->color;
		}

		/** let gnuplot decide the color */
		//void setColorAuto() {this->color = K::GnuplotColor::AUTO();}

	};

}

#endif // GNUPLOTATTRCOLOR_H
