#ifndef GNUPLOTPLOT_H
#define GNUPLOTPLOT_H

#include <vector>
#include <string>

#include "GnuplotDrawable.h"
#include "GnuplotPlotElement.h"

namespace K {

	/**
	 * Plot (gnuplot 2D plot)
	 */
	class GnuplotPlot : public GnuplotDrawable  {

	private:

		/** the elements to draw */
		std::vector<const GnuplotPlotElement*> elements;

	public:

		/** add a to-be-drawn element to this plot */
		void add(const GnuplotPlotElement* elem) {
			elements.push_back(elem);
		}

		/** remove a to-be-drawn element from the plot */
		void remove(const GnuplotPlotElement* elem) {
			elements.erase(std::find(elements.begin(), elements.end(), elem));
		}

		/** get a string of what to draw */
		void addTo(std::stringstream& ss) const override {

			// append general parts beforehand
			for (const GnuplotPlotElement* elem : elements) {
				elem->addGeneralTo(ss);
			}

			ss << "plot ";

			// append the drawing-header for each element
			for (const GnuplotPlotElement* elem : elements) {
				elem->addHeaderTo(ss);
				ss << ", ";
			}
			ss << "\n";

			// append the drawing data for each element
			for (const GnuplotPlotElement* elem : elements) {
				elem->addDataTo(ss);
			}
			ss << "\n\n";

		}

	};


}

#endif // GNUPLOTPLOT_H
