#ifndef GNUPLOTOBJECT_H
#define GNUPLOTOBJECT_H

#include <sstream>

namespace K {

	/**
	 * an object that can be added to a Gnuplot Plot
	 */
	class GnuplotObject {

	public:

		virtual void addTo(std::stringstream& ss, const int id) const = 0;

	};

}

#endif // GNUPLOTOBJECT_H
