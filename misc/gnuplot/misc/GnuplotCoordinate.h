#ifndef GNUPLOTCOORDINATE_H
#define GNUPLOTCOORDINATE_H

#include <string>
#include "../../../Exception.h"

namespace K {

	/** supported types of coordinate systems */
	enum class GnuplotCoordinateSystem {
		FIRST,		// first x or y axis depending on whether setting x or y [= left/bottom axis]
		SECOND,		// second x or y axis depending on whether setting y or y [= right/top axis]
		GRAPH,		// coordinate within the plotting-area [0:1]
		SCREEN,		// coordinate within the whole output image [0:1]
		CHARACTER,	// ???
	};

	/** convert coordinate-system to gnuplot string */
	static inline std::string coordSysToGP(const GnuplotCoordinateSystem sys) {
		switch(sys) {
			case GnuplotCoordinateSystem::FIRST: return "first";
			case GnuplotCoordinateSystem::SECOND: return "second";
			case GnuplotCoordinateSystem::GRAPH: return "graph";
			case GnuplotCoordinateSystem::SCREEN: return "screen";
			case GnuplotCoordinateSystem::CHARACTER: return "chracter";
			default: throw Exception("invalid GnuplotCoordinateSystem");
		}
	}

	/** describes a 1D coordinate within the plot given (x,y) and coordinate system */
	struct GnuplotCoordinate1 {

		float val;
		GnuplotCoordinateSystem sys;

		/** ctor */
		GnuplotCoordinate1(const float val, const GnuplotCoordinateSystem sys) : val(val), sys(sys) {;}

		/** convert to gnuplot syntax */
		std::string toGP() const {
			return coordSysToGP(sys) + " " + std::to_string(val);
		}

	};

	/** describes a 2D coordinate within the plot given (x,y) and coordinate system */
	struct GnuplotCoordinate2 {

		float x;
		GnuplotCoordinateSystem sysX;

		float y;
		GnuplotCoordinateSystem sysY;

		/** ctor */
		GnuplotCoordinate2() :
			x(0), sysX(GnuplotCoordinateSystem::FIRST),
			y(0), sysY(GnuplotCoordinateSystem::FIRST) {;}

		/** ctor */
		GnuplotCoordinate2(const float x, const float y, const GnuplotCoordinateSystem sys) :
			x(x), sysX(sys),
			y(y), sysY(sys) {;}

		/** ctor */
		GnuplotCoordinate2(const float x, const GnuplotCoordinateSystem sysX, const float y, const GnuplotCoordinateSystem sysY) :
			x(x), sysX(sysX),
			y(y), sysY(sysY) {;}


		/** convert to gnuplot syntax */
		std::string toGP() const {
			return
				coordSysToGP(sysX) + " " + std::to_string(x) + "," +
				coordSysToGP(sysY) + " " + std::to_string(y);
		}

	};

	/** describes one 3D coordinate within the plot given (x,y) and coordinate system */
	struct GnuplotCoordinate3 {

		float x;
		GnuplotCoordinateSystem sysX;

		float y;
		GnuplotCoordinateSystem sysY;

		float z;
		GnuplotCoordinateSystem sysZ;

		/** ctor */
		GnuplotCoordinate3(const float x, const float y, const float z, const GnuplotCoordinateSystem sys) :
			x(x), sysX(sys),
			y(y), sysY(sys),
			z(z), sysZ(sys) {;}

		/** ctor */
		GnuplotCoordinate3(const float x, const GnuplotCoordinateSystem sysX, const float y, const GnuplotCoordinateSystem sysY, const float z, const GnuplotCoordinateSystem sysZ) :
			x(x), sysX(sysX),
			y(y), sysY(sysY),
			z(z), sysZ(sysZ) {;}

		/** ctor for 2D points, where 3D is supported */
		GnuplotCoordinate3(const float x, const GnuplotCoordinateSystem sysX, const float y, const GnuplotCoordinateSystem sysY) :
			x(x), sysX(sysX),
			y(y), sysY(sysY),
			z(0), sysZ(GnuplotCoordinateSystem::GRAPH) {;}


		/** convert to gnuplot syntax */
		std::string toGP() const {
			return
				coordSysToGP(sysX) + " " + std::to_string(x) + "," +
				coordSysToGP(sysY) + " " + std::to_string(y) + "," +
				coordSysToGP(sysZ) + " " + std::to_string(z);
		}

	};

}

#endif // GNUPLOTCOORDINATE_H
