#ifndef GNUPLOTPLOTELEMENTIMAGE_H
#define GNUPLOTPLOTELEMENTIMAGE_H


//plot 'stereo1.jpg' binary filetype=jpg with rgbimage

#include "GnuplotPlotElement.h"
#include "GnuplotStructs.h"

#include <vector>

namespace K {

	/**
	 * plot images within 2D plots
	 */
	class GnuplotPlotElementImage : public GnuplotPlotElement {

	private:

		std::vector<GnuplotPoint3> pts;	// (x,y) -> val
		std::string filename;			// (x,y) -> val

	public:

		/** image from file */
		GnuplotPlotElementImage(const std::string& filename) : filename(filename) {
			;
		}

		/** image from (x,y)->val */
		GnuplotPlotElementImage() {
			;
		}

		void add(GnuplotPoint3 pt) {
			pts.push_back(pt);
		}

		void addHeaderTo(std::stringstream& ss, const GnuplotStringMod* mod) const override {
			if (!filename.empty()) {
				std::string filetype = filename.substr(filename.size() - 3, filename.size());
				ss << "'" << filename << "' binary  filetype=" << filetype << " flipy with rgbimage ";
			} else {
				ss << "'-' with image ";
			}
			ss << attrCustom << " ";
			ss << " title '" << mod->modEntryTitle(title) << "'";
		}

		void addDataTo(std::stringstream& ss) const override {
			(void) ss;
			;	// nothing to-do here

			// plot points
//			float oy = NAN;
			for (const GnuplotPoint3& p : pts) {
//				if (oy==oy && oy!=p.y) {
//					ss << "\n\n";
//				}
//				oy = p.y;
				if (p.isEmpty()) {
					ss << "\n";
				} else {
					ss << p.x << ' ' << p.y << ' ' << p.z << "\n";
				}
			}
			ss << "e\n";

		}

		bool isEmpty() const override {
			return false;
		}

	};

}


#endif // GNUPLOTPLOTELEMENTIMAGE_H
