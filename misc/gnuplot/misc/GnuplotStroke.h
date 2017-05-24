#ifndef GNUPLOTSTROKE_H
#define GNUPLOTSTROKE_H

#include <string>
#include "GnuplotDashtype.h"
#include "GnuplotColor.h"

namespace K {

	/** how to stroke elements */
	struct GnuplotStroke {

		/** the stroke-type [none, solid, dashed, ..] */
		GnuplotDashtype type;

		/** stroke's width */
		float width;

		/** the color to use for stroking [if any] */
		GnuplotColor color;


		/** empty ctor */
		GnuplotStroke() : type(GnuplotDashtype::SOLID), width(1.0f), color() {
			;
		}

		/** ctor */
		GnuplotStroke(const GnuplotDashtype type, const float width, const GnuplotColor& color) :
		    type(type), width(width), color(color) {
			;
		}

		/** get the stroke's color */
		GnuplotColor& getColor() {
			return color;
		}

		/** no-stroke */
		static inline GnuplotStroke NONE() {
			return GnuplotStroke(GnuplotDashtype::NONE, 0, GnuplotColor());
		}

		/** set the stroke's color */
		void setColor(const GnuplotColor& color) {
			this->color = color;
		}

		/** set the stroke's width */
		void setWidth(const float w) {
			this->width = w;
		}

		/** set the stroke's type */
		void setType(const GnuplotDashtype& type) {
			this->type = type;
		}

		/** get the stroke's type */
		GnuplotDashtype getType() const {
			return type;
		}

		/** to gnuplot */

		std::string toGP(const bool withColor = true) const {
			std::string res;
			if (!color.isAuto() && withColor) {res += " linecolor " + color.toGP();}
			res += " linewidth " + std::to_string(width);
			res += " dashtype " + dashtypeToGP(type);
			return res;
		}

	};


}

#endif // GNUPLOTSTROKE_H
