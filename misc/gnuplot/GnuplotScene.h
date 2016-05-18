#ifndef GNUPLOTSCENE_H
#define GNUPLOTSCENE_H

#include "GnuplotDrawable.h"
#include "GnuplotObject.h"

namespace K {

	/** combine objects and one drawable (plot/splot) */
	class GnuplotScene : public GnuplotDrawable {

	private:

		std::vector<GnuplotObject*> objects;

		GnuplotDrawable* drawable = nullptr;

	public:

		void addObject(GnuplotObject* obj) {objects.push_back(obj);}

		void setDrawable(GnuplotDrawable* d) {drawable = d;}

		virtual void addTo(std::stringstream& ss) const override {

			// add all objects
			int id = 0;
			for (const GnuplotObject* o : objects) {
				o->addTo(ss, ++id);
			}

			// add the scene
			if (drawable) {drawable->addTo(ss);}

		}

	};


}

#endif // GNUPLOTSCENE_H
