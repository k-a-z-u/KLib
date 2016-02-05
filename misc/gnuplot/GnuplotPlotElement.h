#ifndef GNUPLOTPLOTELEMENT_H
#define GNUPLOTPLOTELEMENT_H

#include <sstream>
#include "GnuplotStructs.h"
#include "attributes/GnuplotAttrTitle.h"
#include "attributes/GnuplotAttrColor.h"
#include "attributes/GnuplotAttrCustom.h"

namespace K {

	/**
	 * an element that can be added to a Gnuplot Plot (2D plot)
	 */
	class GnuplotPlotElement : public GnuplotAttrTitle, public GnuplotAttrColor, public GnuplotAttrCustom {

	public:

		virtual void addHeaderTo(std::stringstream& ss) const = 0;

		virtual void addDataTo(std::stringstream& ss) const = 0;

	};

}

#endif // GNUPLOTPLOTELEMENT_H
