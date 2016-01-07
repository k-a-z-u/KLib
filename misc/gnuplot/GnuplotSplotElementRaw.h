#ifndef GNUPLOTSPLOTELEMENTRAW_H
#define GNUPLOTSPLOTELEMENTRAW_H

#include "GnuplotSplotElement.h"

namespace K {

	/**
	 * helper class for plotting raw data points
	 * within splots (3D)
	 */
	class GnuplotSplotElementRaw : public GnuplotSplotElement {

	protected:

		std::vector<GnuplotPoint3> points;

	public:

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		void addDataTo(std::stringstream& ss) const override {

			for (const GnuplotPoint3 p : points) {
				if (p.isEmpty()) {
					ss << "\n";
				} else {
					ss << p.x << ' ' << p.y << ' ' << p.z << "\n";
				}
			}
			ss << "e\n";

		}

	};

}

#endif // GNUPLOTSPLOTELEMENTRAW_H
