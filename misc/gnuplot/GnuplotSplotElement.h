#ifndef GNUPLOTSPLOTELEMENT_H
#define GNUPLOTSPLOTELEMENT_H

#include <sstream>
#include "GnuplotStructs.h"
#include "attributes/GnuplotAttrTitle.h"
#include "attributes/GnuplotAttrCustom.h"

namespace K {

	/**
	 * an element that can be added to a Gnuplot SPlot (3D plot)
	 */
	class GnuplotSplotElement : public GnuplotAttrTitle, public GnuplotAttrCustom {

	public:

		virtual void addHeaderTo(std::stringstream& ss) const = 0;

		virtual void addDataTo(std::stringstream& ss) const = 0;

	};

}

#endif // GNUPLOTSPLOTELEMENT_H
