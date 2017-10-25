#ifndef GNUPLOTPLOTELEMENTEMPTY_H
#define GNUPLOTPLOTELEMENTEMPTY_H

#include <vector>
#include "GnuplotPlotElementRaw.h"

namespace K {

	/**
	 * if you have a plot consisting of all objects only,
	 * you still need to plot something.
	 * use this and set x,y,z range properly!
	 */
	class GnuplotPlotElementEmpty : public GnuplotPlotElement {

	public:

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			ss << "1/0 notitle";
		}

		virtual void addDataTo(std::stringstream& ss) const override {
			(void) ss;
		}

		virtual bool isEmpty() const {
			return false;
		}

	};

}

#endif // GNUPLOTPLOTELEMENTEMPTY_H
