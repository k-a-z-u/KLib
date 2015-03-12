/*
 * SVGFile.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SVGFILE_H_
#define SVGFILE_H_

//#include "SVGLoader.h"
#include "SVGComposite.h"

namespace K {

class SVGFile {

public:

//	SVGFile(const File& f) : width(0), height(0) {
//		layers = new SVGComposite();
//		SVGLoader::load(f, this);
//	}

	SVGFile() : width(0), height(0) {
		layers = new SVGComposite();
	}

	~SVGFile() {
		delete layers; layers = nullptr;
	}

	/** get all layers */
	SVGComposite* getLayers() {
		return layers;
	}

	double getWidth() const {return width;}
	double getHeight() const {return height;}

private:

	friend class SVGLoader;

	double width;
	double height;

	/** all layers within the svg */
	SVGComposite* layers;

};

}


#endif /* SVGFILE_H_ */
