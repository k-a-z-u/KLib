#ifndef GNUPLOTOBJECTRECTANGLE_H
#define GNUPLOTOBJECTRECTANGLE_H

#include "GnuplotObject.h"
#include "../misc/GnuplotCoordinate.h"
#include "../misc/GnuplotFill.h"
#include "../misc/GnuplotStroke.h"


namespace K {

	/** a rectangle object */
	class GnuplotObjectRectangle : public GnuplotObject {

	private:

		GnuplotCoordinate2 from;
		GnuplotCoordinate2 to;
		GnuplotFill fill;
		GnuplotStroke stroke;

	public:

		/** ctor */
		GnuplotObjectRectangle(const GnuplotCoordinate2& from, const GnuplotCoordinate2& to, const GnuplotFill& fill, const GnuplotStroke& stroke) :
		    from(from), to(to), fill(fill), stroke(stroke) {
			;
		}

	public:

		virtual void addTo(std::stringstream& ss) const override {
			ss << " set object " << _id << " rectangle ";
			ss << " from " << from.toGP();
			ss << " to " << to.toGP();
			ss << getStyle(fill, stroke);
			//ss << " " << fill.toGP();
			//ss << " " << stroke.toGP();
			if (front) {ss << " front ";}
			if (back) {ss << " back ";}
			ss << "\n";
		}

		virtual void removeFrom(std::stringstream& ss) const override {
			ss << " unset object " << _id << "\n";
		}

	};

}

#endif // GNUPLOTOBJECTRECTANGLE_H
