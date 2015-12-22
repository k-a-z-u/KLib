#ifndef GNUPLOTDRAWABLE3D_H
#define GNUPLOTDRAWABLE3D_H

#include <vector>
#include <sstream>

namespace K {



	class GnuplotDrawable3D {




	std::vector<GnuplotPoint3> points;

	public:




		/** get drawing data for splot */
		virtual std::string getData() {

		}

	};

}

#endif // GNUPLOTDRAWABLE3D_H
