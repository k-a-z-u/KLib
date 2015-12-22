#ifndef GNUPLOTSPLOT_H
#define GNUPLOTSPLOT_H

#include <vector>
#include <string>

#include "GnuplotDrawable.h"
#include "GnuplotSplotElement.h"

namespace K {

	/**
	 * SPlot (gnuplot 3D plot)
	 */
	class GnuplotSplot : public GnuplotDrawable  {

	private:

		/** the elements to draw */
		std::vector<const GnuplotSplotElement*> elements;

	public:

		/** add a to-be-drawn element to this splot */
		void add(const GnuplotSplotElement* elem) {
			elements.push_back(elem);
		}

		/** get a string of what to draw */
		void addTo(std::stringstream& ss) const override {

			ss << "splot ";

			// append the drawing-header for each element
			for (const GnuplotSplotElement* elem : elements) {
				elem->addHeaderTo(ss);
				ss << ", ";
			}
			ss << "\n";

			// append the drawing data for each element
			for (const GnuplotSplotElement* elem : elements) {
				elem->addDataTo(ss);
			}
			ss << "\n\n";

		}

	};

}

#endif // GNUPLOTSPLOT_H
