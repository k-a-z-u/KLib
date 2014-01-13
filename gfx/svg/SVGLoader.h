/*
 * SVGLoader.h
 *
 *  Created on: Aug 16, 2013
 *      Author: Frank Ebner
 */

#ifndef SVGLOADER_H_
#define SVGLOADER_H_

#include "../../fs/File.h"
#include "SVGComposite.h"

namespace tinyxml2 {
class XMLElement;
};

namespace K {

class File;
class SVGFile;

/**
 * load floor plan from SVG
 */
class SVGLoader {

protected:

	friend class SVGFile;

	/** load the given file */
	static void load(const File& f, SVGFile* dst);

private:

	/** apply the transform specified within the given node to the (already loaded) obstacles */
	static void applyTransform(const tinyxml2::XMLElement* node, SVGElement* dst);

	/** load all paths within this layer */
	static SVGComposite* loadPaths(SVGFile* dst, const tinyxml2::XMLElement* layer);


};

}

#endif /* SVGLOADER_H_ */
