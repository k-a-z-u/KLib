#ifndef GNUPLOTDRAWABLE_H
#define GNUPLOTDRAWABLE_H

#include <sstream>

namespace K {

	class GnuplotDrawable {

	public:

		/** draw this element by appending its code to the given stream */
		virtual void addTo(std::stringstream& ss) const = 0;

	};

}

#endif // GNUPLOTDRAWABLE_H
