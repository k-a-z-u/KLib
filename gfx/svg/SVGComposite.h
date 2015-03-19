/*
 * SVGComposite.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SVGCOMPOSITE_H_
#define SVGCOMPOSITE_H_

#include <vector>
#include "SVGElement.h"


namespace K {

	class SVGLayer;

	class SVGComposite : public SVGElement {

	public:

		/** dtor */
		virtual ~SVGComposite() {;}

		/** get all child elements */
		const std::vector<SVGElement*>& getChilds() const {
			return childs;
		}

		/** add a new child element */
		void addChild(SVGElement* c) {
			if (!c) {throw "child must not be nullptr!";}
			childs.push_back(c);
		}

		void transform(const double x, const double y) override {
			for (SVGElement* e : childs) {
				e->transform(x,y);
			}
		}

		virtual SVGElementType getType() const override {
			return SVGElementType::COMPOSITE;
		}

		/**
		 * find the first layer within this composite that has the given name.
		 * returns null if none matches
		 */
		K::SVGLayer* getContainedLayerNamed(const std::string& name) const;

	private:

		/** all childs within this composite */
		std::vector<SVGElement*> childs;



	};

}



#include "SVGLayer.h"

K::SVGLayer* K::SVGComposite::getContainedLayerNamed(const std::string& name) const {

	// process each child
	for (K::SVGElement* elSub : getChilds() ) {

		// skip other child-types
		if (elSub->getType() != SVGElementType::LAYER) {continue;}

		// found?
		K::SVGLayer* lay = (K::SVGLayer*) elSub;
		if (lay->getName() == name) {return lay;}

	}

	// not found
	return nullptr;

}




#endif /* SVGCOMPOSITE_H_ */
