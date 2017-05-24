#ifndef GNUPLOTSPLOTELEMENTRAW_H
#define GNUPLOTSPLOTELEMENTRAW_H

#include <vector>
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

		void addDataTo(std::ostream& ss) const override {

			for (const GnuplotPoint3& p : points) {
				if (!p.isEmpty()) {
					ss << p.x << ' ' << p.y << ' ' << p.z;
				};
				ss << "\n";
			}
			ss << "e\n";

		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p) {
			points.push_back(p);
		}

		/** get the number of entries */
		size_t size() const {
			return points.size();
		}

		/** does this element contain any points? */
		bool empty() const override {
			return points.empty();
		}

		/** remove all added elements */
		void clear() {
			points.clear();
		}

	};

}

#endif // GNUPLOTSPLOTELEMENTRAW_H
