/*
 * SVGElement.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SVGELEMENT_H_
#define SVGELEMENT_H_

#include "SVGElementType.h"

namespace K {

class SVGElement {


public:

	/** dtor */
	virtual ~SVGElement() {;}

	/** move this element */
	virtual void transform(double x, double y) {
		(void)x;
		(void)y;
	}

	/** get this element's type */
	virtual SVGElementType getType() = 0;

};

}


#endif /* SVGELEMENT_H_ */
