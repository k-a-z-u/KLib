#ifndef GNUPLOTVIEW_H
#define GNUPLOTVIEW_H

#include <string>

namespace K {

	class GnuplotView3 {

	private:

		float degX = 45;
		float degY = 40;
		float scaleAll = 1.0;
		float scaleZ = 1.0;

	public:

		void setCamera(const float degX, const float degY) {
			this->degX = degX;
			this->degY = degY;
		}

		/** configure z-axis scale [default: 1] */
		void setScaleZ(const float scale) {
			this->scaleZ = scale;
		}

		/** configure overall scale [default: 1] */
		void setScaleAll(const float scale) {
			this->scaleAll = scale;
		}

		void addTo(std::ostream& ss) const {
			ss << "set view " << degX << "," << degY << " ";
			if (scaleAll != 1 || scaleZ != 1) {
				ss << "," << scaleAll << "," << scaleZ << " ";
			}
			ss << "\n";
		}

	};

}

#endif // GNUPLOTVIEW_H
