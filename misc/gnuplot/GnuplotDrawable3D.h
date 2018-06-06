#ifndef GNUPLOTDRAWABLE3D_H
#define GNUPLOTDRAWABLE3D_H

#include <vector>
#include <sstream>

#include "GnuplotStructs.h"

namespace K {



	class GnuplotDrawable3D {




	std::vector<GnuplotPoint3> points;

	public:




		/** get drawing data for splot */
                virtual std::string getData() = 0;

	};

}

#endif // GNUPLOTDRAWABLE3D_H
