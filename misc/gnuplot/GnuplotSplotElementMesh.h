#ifndef GNUPLOTSPLOTELEMENTMESH_H
#define GNUPLOTSPLOTELEMENTMESH_H

#include <vector>
#include "GnuplotSplotElementRaw.h"
#include "attributes/GnuplotAttrColor.h"

namespace K {

	/**
	 * helper class for plotting raw data points
	 * within splots (3D)
	 */
	class GnuplotSplotElementMesh : public GnuplotSplotElement {

	protected:

		std::vector<GnuplotPoint3> points;

	public:

		/** remove all added elements */
		void clear() {
			points.clear();
		}

		/** add a new point to output */
		void add(const GnuplotPoint3 p) {
			points.push_back(p);
		}

		void addHeaderTo(std::ostream& ss) const override {
			ss << "'-' ";
		}

		void addDataTo(std::ostream& ss) const override {

			auto copy = points;
			auto comp = [] (const GnuplotPoint3& p1, const GnuplotPoint3& p2) {
				if (p1.y < p2.y) {return true;}
				if (p1.y == p2.y) {return p1.x < p2.x;}
				return false;
			};
			std::sort(copy.begin(), copy.end(), comp);

			float oy = 0;
			for (const GnuplotPoint3& p : copy) {
				if (p.isEmpty()) {
					ss << "\n";
				} else {
					if (p.y != oy) {ss << "\n"; oy = p.y;}
					ss << p.x << ' ' << p.y << ' ' << p.z << "\n";
				}
			}
			ss << "e\n";

		}

	};

}

#endif // GNUPLOTSPLOTELEMENTMESH_H
