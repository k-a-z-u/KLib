#ifndef GNUPLOTPLOTELEMENTIMAGE_H
#define GNUPLOTPLOTELEMENTIMAGE_H


//plot 'stereo1.jpg' binary filetype=jpg with rgbimage

#include "GnuplotPlotElement.h"

namespace K {

	/**
	 * plot images within 2D plots
	 */
	class GnuplotPlotElementImage : public GnuplotPlotElement {

	private:

		std::string filename;

	public:

		GnuplotPlotElementImage(const std::string& filename) : filename(filename) {
			;
		}

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			std::string filetype = filename.substr(filename.size() - 3, filename.size());
			ss << "'" << filename << "' binary  filetype=" << filetype << " flipy with rgbimage ";
			ss << attrCustom << " ";
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		void addDataTo(std::stringstream& ss) const override {
			(void) ss;
			;	// nothing to-do here
		}

		bool isEmpty() const override {
			return false;
		}

	};

}


#endif // GNUPLOTPLOTELEMENTIMAGE_H
