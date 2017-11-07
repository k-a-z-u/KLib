#ifndef GNUPLOTOBJECTS_H
#define GNUPLOTOBJECTS_H

#include "GnuplotObjectRectangle.h"
#include "GnuplotObjectCircle.h"
#include "GnuplotObjectPolygon.h"
#include "GnuplotObjectArrow.h"

#include <vector>
#include <map>

namespace K {

	class GnuplotObjects {

	private:

		/** all active objects */
		std::map<int, GnuplotObject*> objects;
		mutable std::stringstream pending;

		int nextID = 0;

	public:

		/** dtor */
		~GnuplotObjects() {
			//for (GnuplotObject* o : objects) {delete o;}
			objects.clear();
		}

		/** add a new element */
		void add(GnuplotObject* obj) {
			++nextID;				// next unique ID
			obj->_id = nextID;		// assign unique ID
			objects[nextID] = obj;
		}

		/** overwrite an existing object */
		void set(const int id, GnuplotObject* obj) {
			obj->_id = id;
			objects[id] = obj;
		}

		/** get the object for the given ID */
		GnuplotObject* get(const int id) {
			const auto& it = objects.find(id);
			if (it == objects.end()) {return nullptr;}
			return it->second;
		}

		/** remove the object with the given id */
		void remove(const int id) {
			objects[id]->removeFrom(pending);
			objects.erase(id);
		}

		/** remove the given object */
		void remove(GnuplotObject* obj) {
			remove(obj->_id);
		}

//		void deleteAll() {
//			for (const auto& it : objects) {delete it.second;}
//			objects.clear();
//		}

		void reOrderByZIndex() {

			// sort elements by the manually added z-index
			auto comp = [] (const GnuplotObject* o1, const GnuplotObject* o2) {
				return o1->zIndex < o2->zIndex;
			};

			// sort
			std::vector<GnuplotObject*> copy;
			for (const auto& it : objects) {copy.push_back(it.second);}
			std::sort(copy.begin(), copy.end(), comp);

			// update IDs
			int id = 1;
			for (GnuplotObject* o : copy) {
				o->_id = id; ++id;
			}

		}

	public:


		/** serialize all objects */
		virtual void addTo(std::stringstream& ss) const {

			// pending actions? e.g. removals
			ss << pending.str();
			pending.str("");

			// add elements
			//for (const GnuplotObject* obj : objects) {
			for (const auto& it : objects) {
				it.second->addTo(ss);
			}
			ss << "\n\n";

		}

	};


}

#endif // GNUPLOTOBJECTS_H
