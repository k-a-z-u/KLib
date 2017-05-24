#ifndef GNUPLOTFILL_H
#define GNUPLOTFILL_H

#include <string>
#include "GnuplotFillstyle.h"
#include "GnuplotColor.h"

namespace K {

	/** how to fill elements */
	struct GnuplotFill {

		/** how to fill */
		GnuplotFillStyle style;

		/** alpha */
		float alpha;

		/** the color to use for filling [if any] */
		GnuplotColor color;


		/** empty ctor */
		GnuplotFill() : style(GnuplotFillStyle::EMPTY), alpha(1), color() {;}

		/** ctor */
		GnuplotFill(const GnuplotFillStyle& style, const GnuplotColor& color, const float alpha = 1.0) :
			style(style), alpha(alpha), color(color) {
			;
		}

		void setColor(GnuplotColor c) {
			this->color = c;
		}

		void setStyle(GnuplotFillStyle style) {
			this->style = style;
		}

		void setAlpha(const float alpha) {
			this->alpha = alpha;
		}

		/** no-fill setting */
		static GnuplotFill NONE() {
			return GnuplotFill(GnuplotFillStyle::EMPTY, GnuplotColor());
		}


		/** to gnuplot */
		std::string toGP() const {
			std::string res;
			if (style != GnuplotFillStyle::EMPTY) {
				res += " fillcolor " + color.toGP();
			}
			res += " fillstyle " + fillStyleToGP(style);
			if (style == GnuplotFillStyle::SOLID) {
				res += " " + std::to_string(alpha);
			}
			return res;
		}

	};

}


#endif // GNUPLOTFILL_H
