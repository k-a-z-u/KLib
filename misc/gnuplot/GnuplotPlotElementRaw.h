#ifndef GNUPLOTPLOTELEMENTRAW_H
#define GNUPLOTPLOTELEMENTRAW_H

#include "GnuplotPlotElement.h"
#include <vector>
namespace K {

	/**
	 * helper class for plotting raw data points
	 * within plots (2D)
	 */
	class GnuplotPlotElementRaw : public GnuplotPlotElement {

	protected:

		std::vector<GnuplotPoint2> points;

	public:

		/** nothing to plot? */
		bool isEmpty() const override {
			return points.empty();
		}

		void addDataTo(std::stringstream& ss) const override {

			// plot points
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
