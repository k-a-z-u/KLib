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

		void addHeaderTo(std::stringstream& ss) const override {
			std::string filetype = filename.substr(filename.size() - 3, filename.size());
			ss << "'" << filename << "' binary  filetype=" << filetype << " flipy with rgbimage ";
			ss << attrCustom << " ";
			ss << " title '" << title << "'";
		}

		void addDataTo(std::stringstream& ss) const override {
			;	// nothing to-do here
		}

	};

}


#endif // GNUPLOTPLOTELEMENTIMAGE_H
