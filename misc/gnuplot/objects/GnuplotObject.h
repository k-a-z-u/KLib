#ifndef GNUPLOTOBJECT_H
#define GNUPLOTOBJECT_H

#include "../misc/GnuplotStroke.h"
#include "../misc/GnuplotFill.h"

namespace K {

	class GnuplotObject {

	protected:

		friend class GnuplotObjects;

		/** set internally when added to the plot */
		int _id;

		/** object in the front/back? */
		bool front = false;
		bool back = false;

		/** z-index for manual draw-ordering */
		float zIndex = 0;

		/** object enabled/disabled? */
		bool enabled = true;

	public:

		/** dtor */
		virtual ~GnuplotObject() {;}

		/** object in the front of others? */
		void setFront(const bool front) {
			this->front = front;
		}

		/** object behind others? */
		void setBack(const bool back) {
			this->back = back;
		}

		/**
		 * as gnuplot does not support depth-ordering,
		 * set a z-index to manually influence the drawing order
		 */
		void setZIndex(const float zIndex) {
			this->zIndex = zIndex;
		}

		/** enable/disable the object */
		void setEnabled(const bool enabled) {
			this->enabled = enabled;
		}

	public:

		/** attach object's string to the given stream */
		virtual void addTo(std::stringstream& ss) const = 0;

		/** remove object */
		virtual void removeFrom(std::stringstream& ss) const = 0;
		
		int getID() const
		{
			return _id;
		}

	protected:

		/** unfortunately gnuplot mixes stroke and fill, therefore we need this function */
		static inline std::string getStyle(const GnuplotFill& fill, const GnuplotStroke& stroke) {

			std::string res;

			if (fill.style != GnuplotFillStyle::EMPTY) {
				res += " fillcolor " + fill.color.toGP();
			}
			res += " fillstyle " + fillStyleToGP(fill.style);
			if (fill.style == GnuplotFillStyle::SOLID) {
				res += " " + std::to_string(fill.alpha);
			}

			res += (stroke.getType() == GnuplotDashtype::NONE) ? (" noborder ") : (" border ");

			if (!stroke.color.isAuto()) {res += " linecolor " + stroke.color.toGP();}
			if (stroke.width != 1) {res += " linewidth " + std::to_string(stroke.width);}

			if (stroke.type != GnuplotDashtype::NONE && stroke.type != GnuplotDashtype::SOLID) {
				res += " dashtype " + dashtypeToGP(stroke.type);
			}

			return res;

		}

	};

}

#endif // GNUPLOTOBJECT_H
