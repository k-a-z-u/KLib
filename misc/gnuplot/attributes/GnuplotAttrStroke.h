#ifndef GNUPLOTATTRSTROKE_H
#define GNUPLOTATTRSTROKE_H

#include "../misc/GnuplotStroke.h"

namespace K {

	/**
	 * base-class for elements that have a stroke
	 */
	class GnuplotAttrStroke {

	protected:

		GnuplotStroke stroke;

	public:

		/** get the stroke settings */
		GnuplotStroke& getStroke() {
			return stroke;
		}

		/** set the stroke settings */
		void setStroke(const GnuplotStroke& stroke) {
			this->stroke = stroke;
		}

	};

}


#endif // GNUPLOTATTRSTROKE_H
