#ifndef GNUPLOTSPLOTELEMENT_H
#define GNUPLOTSPLOTELEMENT_H

#include <ostream>
#include "GnuplotStructs.h"
#include "attributes/GnuplotAttrTitle.h"
#include "attributes/GnuplotAttrCustom.h"
#include "misc/GnuplotStringMod.h"

namespace K {

	/**
	 * an element that can be added to a Gnuplot SPlot (3D plot)
	 */
	class GnuplotSplotElement : public GnuplotAttrTitle, public GnuplotAttrCustom {

	public:

		virtual void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const = 0;

		virtual void addDataTo(std::ostream& ss) const = 0;

		virtual bool empty() const = 0;

	};

}

#endif // GNUPLOTSPLOTELEMENT_H
