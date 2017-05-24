#ifndef GNUPLOTOBJECTPOLYGON_H
#define GNUPLOTOBJECTPOLYGON_H

#include "GnuplotObject.h"
#include "../misc/GnuplotCoordinate.h"
#include "../misc/GnuplotFill.h"
#include "../misc/GnuplotStroke.h"
#include "../GnuplotStructs.h"
#include <vector>

namespace K {

	/** a rectangle object */
	class GnuplotObjectPolygon : public GnuplotObject {

	private:

		GnuplotFill fill;
		GnuplotStroke stroke;
		std::vector<GnuplotCoordinate3> points;
		bool front = false;

	public:

		/** empty ctor */
		GnuplotObjectPolygon() : fill(), stroke() {
			;
		}

		/** ctor */
		GnuplotObjectPolygon(const GnuplotFill& fill, const GnuplotStroke& stroke) : fill(fill), stroke(stroke) {
			;
		}

		GnuplotFill& getFill() {
			return fill;
		}

		void setFill(const GnuplotFill& f) {
			this->fill = f;
		}

		GnuplotStroke& getStroke() {
			return stroke;
		}

		void setStroke(const GnuplotStroke& s) {
			this->stroke = s;
		}

		void add(const GnuplotCoordinate3& coord) {
			points.push_back(coord);
		}

		void add(const GnuplotCoordinate2& coord) {
			points.push_back(GnuplotCoordinate3(coord.x, coord.sysX, coord.y, coord.sysY));
		}

		void close() {
			points.push_back(points.front());
		}

	public:

		virtual void addTo(std::stringstream& ss) const override {

			if (!enabled) {
				ss << " unset object " << _id << "\n";
			} else {

				ss << " set object " << _id << " polygon ";
				for (size_t i = 0; i < points.size(); ++i) {
					ss << ( i==0 ? "from " : "to ");
					ss << points[i].toGP();
				}
				//ss << " " << fill.toGP();
				//ss << " " << stroke.toGP();
				ss << getStyle(fill, stroke);
				if (front) {ss << " front ";}
				ss << "\n";

			}

		}

		virtual void removeFrom(std::stringstream& ss) const override {
			ss << " unset object " << _id << "\n";
		}

	};

}

#endif // GNUPLOTOBJECTPOLYGON_H
