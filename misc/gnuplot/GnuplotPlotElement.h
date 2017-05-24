#ifndef GNUPLOTPLOTELEMENT_H
#define GNUPLOTPLOTELEMENT_H

#include <sstream>
#include "GnuplotStructs.h"
#include "attributes/GnuplotAttrTitle.h"
#include "attributes/GnuplotAttrColor.h"
#include "attributes/GnuplotAttrCustom.h"
#include "misc/GnuplotStringMod.h"

namespace K {

	/**
	 * an element that can be added to a Gnuplot Plot (2D plot)
	 */
	class GnuplotPlotElement : public GnuplotAttrTitle, public GnuplotAttrCustom {

	protected:

		int useAxisX = 1;
		int useAxisY = 1;

	public:

		/** dtor */
		virtual ~GnuplotPlotElement() {;}

		/** configure the to-be-used axis [x1,x2] and [y1,y2] */
		void setUseAxis(const int x, const int y) {
			this->useAxisX = x;
			this->useAxisY = y;
		}

		/** nothing to plot? */
		virtual bool isEmpty() const = 0;

		virtual void addGeneralTo(std::stringstream& ss) const {(void)ss;}

		virtual void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const = 0;

		virtual void addDataTo(std::stringstream& ss) const = 0;

	protected:

		std::string getUseAxis() const {
			return std::string() + "axes " + "x" + std::to_string(useAxisX) + "y" + std::to_string(useAxisY) + " ";
		}

	};

}

#endif // GNUPLOTPLOTELEMENT_H
