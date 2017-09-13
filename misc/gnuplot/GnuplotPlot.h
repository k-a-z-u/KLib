#ifndef GNUPLOTPLOT_H
#define GNUPLOTPLOT_H

#include <vector>
#include <string>

#include "GnuplotDrawable.h"
#include "GnuplotPlotElement.h"
#include "objects/GnuplotObjects.h"
#include "misc/GnuplotKey.h"
#include "misc/GnuplotAxis.h"
#include "misc/GnuplotStringMod.h"
#include "misc/GnuplotMargin.h"

namespace K {

	/**
	 * Plot (gnuplot 2D plot)
	 */
	class GnuplotPlot : public GnuplotDrawable  {


	private:

		/** the elements to draw */
		std::vector<const GnuplotPlotElement*> elements;

		/** other objects to draw [those require special handling] */
		GnuplotObjects objects;

		GnuplotAxis axisX;
		GnuplotAxis axisX2;
		GnuplotAxis axisY;
		GnuplotAxis axisY2;
		GnuplotAxis axisCB;

		GnuplotStringModNone* MOD_NONE = new GnuplotStringModNone();
		GnuplotStringMod* mod = MOD_NONE;

		GnuplotMargin margin;

		std::stringstream custom;

		bool gridVisible = false;

		GnuplotKey key;

	public:

		/** ctor */
		GnuplotPlot() : axisX("x", true), axisX2("x2", false), axisY("y", true), axisY2("y2", false), axisCB("cb", true) {
			;
		}

		/** add a to-be-drawn element to this plot */
		void add(const GnuplotPlotElement* elem) {
			elements.push_back(elem);
		}

		/** remove a to-be-drawn element from the plot */
		void remove(const GnuplotPlotElement* elem) {
			elements.erase(std::find(elements.begin(), elements.end(), elem));
		}

		/** access to additional custom commands/attributes/settings */
		std::stringstream& getCustom() {
			return custom;
		}

		/** get the first x axis */
		GnuplotAxis& getAxisX() {
			return axisX;
		}

		/** get the second x axis */
		GnuplotAxis& getAxisX2() {
			return axisX2;
		}

		/** get the first y axis */
		GnuplotAxis& getAxisY() {
			return axisY;
		}

		/** get the second y axis */
		GnuplotAxis& getAxisY2() {
			return axisY2;
		}

		/** get the cb axis */
		GnuplotAxis& getAxisCB() {
			return axisCB;
		}


		/** configure the to-be-used string modifier (e.g. for latex) */
		void setStringMod(GnuplotStringMod* mod) {
			this->mod = mod;
		}

		/** whether to show the background grid */
		void setGrid(const bool show) {
			this->gridVisible = show;
		}

		/** get the plot's key */
		GnuplotKey& getKey() {
			return key;
		}

		/** get special objects */
		GnuplotObjects& getObjects() {
			return objects;
		}

		/** get the plot's margins */
		GnuplotMargin& getMargin() {
			return margin;
		}



	public:

		/** get a string of what to draw */
		void addTo(std::stringstream& ss) const override {

			// axis setup
			axisX.addTo(ss, mod);
			axisX2.addTo(ss, mod);
			axisY.addTo(ss, mod);
			axisY2.addTo(ss, mod);
			axisCB.addTo(ss, mod);

			// background grid?

			if (gridVisible)		{ss << "set grid front\n";}	// TODO

			// key?
			key.addTo(ss);

			margin.addTo(ss);

			// append general parts beforehand
			for (const GnuplotPlotElement* elem : elements) {
				elem->addGeneralTo(ss);
			}

			// plot objects
			objects.addTo(ss);

			ss << custom.str();

			ss << "plot ";

			// append the drawing-header for each element
			for (const GnuplotPlotElement* elem : elements) {
				if (elem->isEmpty()) {continue;}
				elem->addHeaderTo(ss, mod);
				ss << ", ";
			}
			ss << "\n";

			// append the drawing data for each element
			for (const GnuplotPlotElement* elem : elements) {
				if (elem->isEmpty()) {continue;}
				elem->addDataTo(ss);
			}
			ss << "\n\n";

		}

	};


}

#endif // GNUPLOTPLOT_H
