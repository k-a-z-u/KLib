#ifndef GNUPLOTLEGEND_H
#define GNUPLOTLEGEND_H

#include <string>
#include "GnuplotCoordinate.h"

namespace K {

	/** describes the plot's key */
	class GnuplotKey {

	public:

		enum class Hor {
			LEFT,
			CENTER,
			RIGHT,
		};

		enum class Ver {
			TOP,
			CENTER,
			BOTTOM,
		};

	private:

		bool visible = false;
		float samplen = 1.0;
		float widthIncrement = 0;

		struct Position {

			bool useHorVer = true;
			Hor hor = Hor::RIGHT;
			Ver ver = Ver::TOP;
			GnuplotCoordinate2 coord;

			void addTo(std::stringstream& dst) const {

				if (useHorVer) {
					switch(hor) {
						case Hor::LEFT:		dst << " left "; break;
						case Hor::CENTER:	dst << " center "; break;
						case Hor::RIGHT:	dst << " right "; break;
					}
					switch(ver) {
						case Ver::TOP:		dst << " top "; break;
						case Ver::CENTER:	dst << " center "; break;
						case Ver::BOTTOM:	dst << " bottom "; break;
					}
				} else {
					dst << " at " << coord.toGP();
				}
			}

		} pos;

	public:



		/** make the key (in)visible */
		void setVisible(const bool visible) {
			this->visible = visible;
		}

		/** set the length/size of the indicators for each key */
		void setSampleLength(const float length) {
			this->samplen = length;
		}

		/** change the automatic width by the given amount */
		void setWidthIncrement(const float widthInc) {
			this->widthIncrement = widthInc;
		}

		/** position the key at fixed locations */
		void setPosition(const Hor hor, const Ver ver) {
			this->pos.hor = hor;
			this->pos.ver = ver;
			this->pos.useHorVer = true;
		}

		/** position the key at fixed locations */
		void setPosition(const GnuplotCoordinate2 coord) {
			this->pos.coord = coord;
			this->pos.useHorVer = false;
		}

	public:

		/** convert to gnuplot commands */
		void addTo(std::stringstream& ss) const {
			if (visible) {
				ss << "set key ";
				ss << " samplen " << samplen;
				pos.addTo(ss);
				ss << " width " << widthIncrement;
			} else {
				ss << "unset key ";
			}
			ss << "\n";
		}

	};

}

#endif // GNUPLOTLEGEND_H
