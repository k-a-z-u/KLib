#ifndef GNUPLOTOBJECTPOLYGON3_H
#define GNUPLOTOBJECTPOLYGON3_H

#include <vector>
#include "GnuplotObject.h"
#include "GnuplotStructs.h"

namespace K {

	class GnuplotObjectPolygon3 : public GnuplotObject {

	private:

		std::string fillStyle = "transparent";	// or solid
		std::string fillColor = "rgb 'red'";
		std::vector<GnuplotPoint3> points;

	public:

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
