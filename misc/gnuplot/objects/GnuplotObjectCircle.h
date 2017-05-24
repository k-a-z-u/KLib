#ifndef GNUPLOTOBJECTCIRCLE_H
#define GNUPLOTOBJECTCIRCLE_H


#include "GnuplotObject.h"
#include "../misc/GnuplotCoordinate.h"
#include "../misc/GnuplotFill.h"
#include "../misc/GnuplotStroke.h"

namespace K {

	/** a rectangle object */
	class GnuplotObjectCircle : public GnuplotObject {

	private:

		GnuplotCoordinate2 at;
		GnuplotCoordinate1 radius;
		GnuplotFill fill;
		GnuplotStroke stroke;

	public:

		/** ctor */
		GnuplotObjectCircle(const GnuplotCoordinate2& at, const GnuplotCoordinate1& radius) :
			at(at), radius(radius), fill(), stroke() {
			;
		}

		GnuplotObjectCircle(const GnuplotCoordinate2& at, const GnuplotCoordinate1& radius, const GnuplotFill& fill, const GnuplotStroke& stroke) :
			at(at), radius(radius), fill(fill), stroke(stroke) {
			;
		}

	public:

		virtual void addTo(std::stringstream& ss) const override {
			ss << "set object " << _id << " circle";
			ss << " at " << at.toGP();
			ss << " size " << radius.toGP();
			ss << " " << getStyle(fill, stroke);
			ss << "\n";
		}

		virtual void removeFrom(std::stringstream& ss) const override {
			ss << " unset object " << _id << "\n";
		}

	};

}


#endif // GNUPLOTOBJECTCIRCLE_H
