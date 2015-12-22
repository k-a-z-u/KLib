#ifndef GNUPLOTMULTIPLOT_H
#define GNUPLOTMULTIPLOT_H

#include "GnuplotDrawable.h"
#include <vector>

namespace K {

	class GnuplotMultiplot : public GnuplotDrawable {

	private:

		int rows;
		int cols;
		std::vector<GnuplotDrawable*> elements;

	public:

		/** ctor */
		GnuplotMultiplot(const int rows, const int cols) : rows(rows), cols(cols) {
			;
		}

		/** add a new sub-plot */
		void add(GnuplotDrawable* d) {
			elements.push_back(d);
		}

		virtual void addTo(std::stringstream& ss) const override {
			ss << "set multiplot layout " << rows << ',' << cols << "\n";
			for (const GnuplotDrawable* d : elements) {
				d->addTo(ss);
			}
			ss << "unset multiplot\n";
		}

	};

}

#endif // GNUPLOTMULTIPLOT_H
