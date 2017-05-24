#ifndef GNUPLOTOBJECTARROW_H
#define GNUPLOTOBJECTARROW_H

#include "GnuplotObject.h"
#include "../misc/GnuplotCoordinate.h"
#include "../misc/GnuplotFill.h"
#include "../misc/GnuplotStroke.h"

namespace K {

	/** a rectangle object */
	class GnuplotObjectArrow : public GnuplotObject {

	public:

		enum class Head {
			NONE,
			FRONT,
			BACK,
			BOTH,
		};

	private:

		GnuplotCoordinate3 from;
		GnuplotCoordinate3 to;
		GnuplotStroke stroke;
		Head head;

	public:

		/** ctor */
		GnuplotObjectArrow(const GnuplotCoordinate3& from, const GnuplotCoordinate3& to) :
			from(from), to(to), stroke(), head(Head::FRONT) {
			;
		}

		/** ctor */
		GnuplotObjectArrow(const GnuplotCoordinate3& from, const GnuplotCoordinate3& to, const GnuplotStroke& stroke, const Head head) :
			from(from), to(to), stroke(stroke), head(head) {
			;
		}

		/** ctor */
		GnuplotObjectArrow(const GnuplotCoordinate2& from, const GnuplotCoordinate2& to, const GnuplotStroke& stroke, const Head head) :
		    from(from.x, from.sysX, from.y, from.sysY), to(to.x, from.sysX, to.y, from.sysY), stroke(stroke), head(head) {
			;
		}

		/** arrow-head configuration */
		void setHead(const Head head) {
			this->head = head;
		}

	public:

		virtual void addTo(std::stringstream& ss) const override {
			ss << " set arrow " << _id;
			ss << " from " << from.toGP();
			ss << " to " << to.toGP();
			ss << " " << stroke.toGP();
			ss << " " << headToGP(head);
			ss << " back ";
			ss << "\n";
		}

		virtual void removeFrom(std::stringstream& ss) const override {
			ss << " unset arrow " << _id << "\n";
		}

	private:

		static inline std::string headToGP(const Head head) {
			switch(head) {
				case Head::NONE: return "nohead";
				case Head::FRONT: return "head";
				case Head::BACK: return "backhead";
				case Head::BOTH: return "heads";
				default: throw Exception("invalid GnuplotObjectArrow::Head");
			}
		}

	};

}

#endif // GNUPLOTOBJECTARROW_H
