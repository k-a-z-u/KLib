#ifndef GNUPLOTDASHTYPE_H
#define GNUPLOTDASHTYPE_H

#include <string>

namespace K {

	/** supported dashtypes for the stroke */
	enum class GnuplotDashtype {
		NONE,
		SOLID,
		DASHED,
		DOTTED,
		DASH_DOT,
		DASH_DOT_DOT,
	};

	/** convert to gnuplot string */
	static inline std::string dashtypeToGP(const GnuplotDashtype type) {
		return std::to_string((int)type);
	}

}

#endif // GNUPLOTDASHTYPE_H
