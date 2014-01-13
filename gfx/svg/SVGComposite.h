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

class SVGComposite : public SVGElement {

public:

	/** get all child elements */
	const std::vector<SVGElement*>& getChilds() const {
		return childs;
	}

	/** add a new child element */
	void addChild(SVGElement* c) {
		childs.push_back(c);
	}

	void transform(double x, double y) {
		for (SVGElement* e : childs) {
			e->transform(x,y);
		}
	}


private:

	/** all childs within this composite */
	std::vector<SVGElement*> childs;

};

}


#endif /* SVGCOMPOSITE_H_ */
