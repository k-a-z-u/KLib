#ifndef GNUPLOTOBJECTPOLYGON3_H
#define GNUPLOTOBJECTPOLYGON3_H

#include <vector>
#include "GnuplotObject.h"
#include "GnuplotStructs.h"

#include <cstring>
#include <string.h>

namespace K {

	class GnuplotObjectPolygon3 : public GnuplotObject {

	private:

		std::string fillStyle = "solid";	// or solid
		std::string fillColor = "rgb 'red'";
		std::vector<GnuplotPoint3> points;

	public:

		void setFillColorHEX(const std::string& hex) {
			fillColor = "rgb '" + hex + "'";
		}

		void setFillColorRGB(const int r, const int g, const int b) {
			char buf[8];
			sprintf(buf, "#%02X%02X%02X", r, g, b);
			setFillColorHEX(buf);
		}

		void addTo(std::stringstream& ss, const int id) const override {

			ss << "set object " << id << " polygon ";

			for (size_t i = 0; i < points.size(); ++i) {
				if		(i == 0)				{ss << "from ";} else {ss << "to ";}
				ss << points[i].x << ',' << points[i].y << ',' << points[i].z << ' ';
			}
			if (!fillStyle.empty()) {ss << " fillstyle " << fillStyle;}
			if (!fillColor.empty()) {ss << " fillcolor " << fillColor;}
			ss << "\n";

		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p) {
			points.push_back(p);
		}

		/** remove all points */
		void clear() {
			points.clear();
		}

	};

}

#endif // GNUPLOTOBJECTPOLYGON3_H
