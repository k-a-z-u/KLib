#ifndef GNUPLOTSPLOT_H
#define GNUPLOTSPLOT_H

#include <vector>
#include <string>
#include <sstream>

#include "GnuplotDrawable.h"
#include "GnuplotSplotElement.h"
#include "objects/GnuplotObjects.h"
#include "misc/GnuplotKey.h"
#include "misc/GnuplotAxis.h"
#include "misc/GnuplotStringMod.h"
#include "misc/GnuplotView.h"
#include "misc/GnuplotMargin.h"

namespace K {

	/**
	 * SPlot (gnuplot 3D plot)
	 */
	class GnuplotSplot : public GnuplotDrawable  {

	private:

		/** the elements to draw */
		std::vector<const GnuplotSplotElement*> elements;

		GnuplotAxis axisX;
		GnuplotAxis axisY;
		GnuplotAxis axisZ;
		GnuplotAxis axisCB;
		float ticslevel = 0;
		GnuplotView3 view;

		GnuplotStringModNone* MOD_NONE = new GnuplotStringModNone();
		GnuplotStringMod* mod = MOD_NONE;

		/** the plot's key */
		GnuplotKey key;

		/** other objects to draw [those require special handling] */
		GnuplotObjects objects;

		GnuplotMargin margin;

		/** custom commands/attributes/settings */
		std::stringstream custom;

		std::string title;

	public:

		/** ctor */
		GnuplotSplot() : axisX("x", true), axisY("y", true), axisZ("z", true), axisCB("cb", true) {
			;
		}

		/** add a to-be-drawn element to this splot */
		void add(const GnuplotSplotElement* elem) {
			elements.push_back(elem);
		}


		/** remove an element from this splot */
		void remove(const GnuplotSplotElement* elem) {
			elements.erase(std::remove(elements.begin(), elements.end(), elem), elements.end());
		}

		/** get the first x axis */
		GnuplotAxis& getAxisX() {
			return axisX;
		}

		/** get the first y axis */
		GnuplotAxis& getAxisY() {
			return axisY;
		}

		/** get the z axis */
		GnuplotAxis& getAxisZ() {
			return axisZ;
		}

		/** get the cb axis */
		GnuplotAxis& getAxisCB() {
			return axisCB;
		}

		/** set the plot's title */
		void setTitle(const std::string& title) {
			this->title = title;
		}

		/** where the z-axis starts */
		void setTicslevel(const float val) {
			this->ticslevel = val;
		}

		/** get the plot's view config */
		GnuplotView3& getView() {
			return view;
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

		/** configure the to-be-used string modifier (e.g. for latex) */
		void setStringMod(GnuplotStringMod* mod) {
			this->mod = mod;
		}

		/** access to additional custom commands/attributes/settings */
		std::stringstream& getCustom() {
			return custom;
		}

		/** get a string of what to draw */
		void addTo(std::stringstream& ss) const override {

			// key?
			key.addTo(ss);

			// axis setup
			axisX.addTo(ss, mod);
			axisY.addTo(ss, mod);
			axisZ.addTo(ss, mod);
			axisCB.addTo(ss, mod);

			// other
			view.addTo(ss);
			ss << "set ticslevel " << ticslevel << "\n";

			margin.addTo(ss);

			// title?
			if (!title.empty())		{ss << "set title '" << mod->modTitle(title) << "'\n";}

			// custom settings?
			ss << custom.str();

			// plot objects
			objects.addTo(ss);

			ss << "splot \\\n";

			// append the drawing-header for each element
			int cnt = 0;
			for (const GnuplotSplotElement* elem : elements) {
				if (!elem->empty()) {
					if (cnt != 0) {ss << ",\\\n";}
					elem->addHeaderTo(ss, mod);
					++cnt;
				}
			}
			ss << "\n";

			// append the drawing data for each element
			for (const GnuplotSplotElement* elem : elements) {
				if (!elem->empty()) {
					elem->addDataTo(ss);
				}
			}
			ss << "\n\n";

		}

	};

}

#endif // GNUPLOTSPLOT_H
