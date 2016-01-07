#ifndef GNUPLOTSTRUCTS_H
#define GNUPLOTSTRUCTS_H

#include <cmath>

namespace K {

	/** point for 2D plots */
	struct GnuplotPoint2 {

		float x;
		float y;

		GnuplotPoint2(const float x, const float y) : x(x), y(y) {;}

		static GnuplotPoint2 getEmpty() {return GnuplotPoint2(NAN, NAN);}

		bool isEmpty() const {return x != x;}

	};

	/** point for 3D plots */
	struct GnuplotPoint3 {

		float x;
		float y;
		float z;

		GnuplotPoint3(const float x, const float y, const float z) : x(x), y(y), z(z) {;}

		GnuplotPoint3 operator + (const GnuplotPoint3& o) const	{return GnuplotPoint3(x+o.x, y+o.y, z+o.z);}
		GnuplotPoint3 operator * (const float f) const			{return GnuplotPoint3(x*f, y*f, z*f);}

		static GnuplotPoint3 getEmpty() {return GnuplotPoint3(NAN, NAN, NAN);}

		bool isEmpty() const {return x != x;}

	};

}

#endif // GNUPLOTSTRUCTS_H
