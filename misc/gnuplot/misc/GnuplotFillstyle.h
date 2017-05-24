#ifndef GNUPLOTFILLSTYLE_H
#define GNUPLOTFILLSTYLE_H

#include "../../../Exception.h"
#include <string>

namespace K {

	/** supported styles for filling objects */
	enum class GnuplotFillStyle {
		EMPTY,			// do not fill
		SOLID,			// solid fill
		PATTERN,		// fill with pattern
		EMPTY_BORDER,	// HACK for drawing lines using polygons
	};

	/** convert coordinate-system to gnuplot string */
	static inline std::string fillStyleToGP(const GnuplotFillStyle sys) {
		switch(sys) {
			case GnuplotFillStyle::EMPTY: return "empty";
			case GnuplotFillStyle::SOLID: return "solid";
			case GnuplotFillStyle::PATTERN: return "pattern";
			case GnuplotFillStyle::EMPTY_BORDER: return "empty";
			default: throw Exception("invalid GnuplotFillStyle");
		}
	}

}

#endif // GNUPLOTFILLSTYLE_H
