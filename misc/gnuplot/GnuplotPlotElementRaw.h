#ifndef GNUPLOTPLOTELEMENTRAW_H
#define GNUPLOTPLOTELEMENTRAW_H

#include "GnuplotPlotElement.h"

namespace K {

	/**
	 * helper class for plotting raw data points
	 * within plots (2D)
	 */
	class GnuplotPlotElementRaw : public GnuplotPlotElement {

	protected:

		std::vector<GnuplotPoint2> points;

	public:

		void addDataTo(std::stringstream& ss) const override {

			for (const GnuplotPoint2& p : points) {
				if (p.isEmpty()) {
					ss << "\n";
				} else {
					ss << p.x << ' ' << p.y << "\n";
				}
			}
			ss << "e\n";

		}

	};

}

#endif // GNUPLOTPLOTELEMENTRAW_H
