/*
 * SVGLayer.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SVGLAYER_H_
#define SVGLAYER_H_

#include "SVGComposite.h"

namespace K {

/**
 * represents a layer within a svg file
 * containing several other layers or other elements
 */
class SVGLayer : public SVGComposite {

public:

	/** ctor */
	SVGLayer(const std::string& name) : name(name) {;}

	/** dtor */
	virtual ~SVGLayer() {;}

	/** get the layer's name */
	const std::string& getName() const {
		return name;
	}

	virtual SVGElementType getType() override {
		return SVGElementType::LAYER;
	}

private:


	/** the layer's name */
	std::string name;

};

}


#endif /* SVGLAYER_H_ */
