#ifndef GNUPLOTSPLOTELEMENTEMPTY_H
#define GNUPLOTSPLOTELEMENTEMPTY_H

#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrStroke.h"
#include "misc/GnuplotStroke.h"

namespace K {

	/**
	 * if you have a plot consisting of all objects only,
	 * you still need to plot something.
	 * use this and set x,y,z range properly!
	 */
	class GnuplotSplotElementEmpty : public GnuplotSplotElement {

	public:

		void addHeaderTo(std::ostream& ss, const GnuplotStringMod* mod) const override {
			ss << "1/0 notitle";
		}

		virtual void addDataTo(std::ostream& ss) const {
			(void) ss;
		}

		virtual bool empty() const {
			return false;		}

	};

}

#endif // GNUPLOTSPLOTELEMENTEMPTY_H
