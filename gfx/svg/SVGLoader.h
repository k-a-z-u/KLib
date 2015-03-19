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

#include <string>

#include "SVGFile.h"
#include "SVGLoader.h"
#include "SVGLayer.h"
#include "SVGPath.h"
#include "../../string/String.h"

#include "../../inc/tinyxml/tinyxml2.h"

#define PIXEL_TO_CM		1

namespace K {


	/**
	 * load floor plan from SVG
	 */
	class SVGLoader {

	public:

		//friend class SVGFile;

		/** load the given file */
		static void load(const File& f, SVGFile* dst);


	private:

		/** apply the transform specified within the given node to the (already loaded) obstacles */
		static void applyTransform(const tinyxml2::XMLElement* node, SVGElement* dst);

		/** load all paths within this layer */
		static SVGComposite* loadPaths(SVGFile* dst, const tinyxml2::XMLElement* layer);


	};



	void SVGLoader::load(const File& f, SVGFile* svgFile) {

		// sanity check
		if (!f.exists()) {throw "error while loading file";} //throw Exception("could not open floorplan file: " + file.getAbsolutePath());}

		// open map file
		tinyxml2::XMLDocument doc;
		doc.LoadFile(f.getAbsolutePath().c_str());

		const tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");

		// load width/height and convert them to centimeters
		svgFile->width = atof(svg->Attribute("width")) * PIXEL_TO_CM;
		svgFile->height = atof(svg->Attribute("height")) * PIXEL_TO_CM;

		// get the first layer
		const tinyxml2::XMLElement* layer = svg->FirstChildElement("g");

		// process all layers
		while (layer) {

			// load all paths
			SVGComposite* c = loadPaths(svgFile, layer);
			if (c) {svgFile->layers->addChild(c);}

			// next layer
			layer = layer->NextSiblingElement("g");

		}

	}

	/** apply the transform specified within the given node to the (already loaded) obstacles */
	void SVGLoader::applyTransform(const tinyxml2::XMLElement* node, SVGElement* dst) {

		// get transform attribute (if any)
		const char* tmp = node->Attribute("transform");
		if (tmp == nullptr) {return;}

		// get as string
		std::string transform(tmp);

		// sanity check
		if (!String::startsWith(transform, "translate(")) {
			throw "unsupported transform type: " + transform;
		}

		// split the string
		size_t start = transform.find('(');
		size_t split = transform.find(',', start);
		size_t end = transform.find(')', split);

		std::string xs = transform.substr(start+1, split - (start+1) );
		std::string ys = transform.substr(split+1, end - (split+1) );

		double x = atof( xs.c_str() ) * PIXEL_TO_CM;
		double y = atof( ys.c_str() ) * PIXEL_TO_CM;

		dst->transform(x,y);

	}

	/** load all paths within this layer */
	SVGComposite* SVGLoader::loadPaths(SVGFile* svgFile, const tinyxml2::XMLElement* layer) {

		// get the layers name
		std::string name = layer->Attribute("inkscape:label");
		SVGLayer* dst = new SVGLayer(name);

		// get all path entries for this layer (if any)
		const tinyxml2::XMLElement* path = layer->FirstChildElement("path");
		if (!path) {return nullptr;}

		// temporal storage
		std::vector<std::string> points;
		std::vector<std::string> styles;
		Point p1;
		Point p2;

		// process all paths
		while (path) {

			// create a new path and add it to the current layer
			SVGPath* sPath = new SVGPath();
			dst->addChild(sPath);

			// style
			styles.clear();
			std::string styleStr = path->Attribute("style");
			String::split(styleStr, styles, ';');

			// parse all attributes
			for (std::string& s : styles) {
				size_t idx = s.find(':');
				if (idx == s.npos) {continue;}
				std::string key = s.substr(0,idx);
				std::string val = s.substr(idx+1);
				if (key == "stroke") {sPath->lineColor.setFromHex(val);}
			}

			// point-string
			std::string pointStr = path->Attribute("d");

			// split all points
			points.clear();
			String::split(pointStr, points, ' ');

			// mode (describes whether subsequent points are absolute or relative to the first one)
			char mode = points[0][0];


			// reset starting point (necessary for relative coordinates)
			p2.set(0, svgFile->height);

			// process each point
			int cnt = 0;
			for (const std::string& str : points) {

				// find splitting ','
				size_t pos = str.find(',');
				if (pos == str.npos) {continue;}//throw Exception("missing ',' within path");}

				// get x and y
				std::string x = str.substr(0, pos);
				std::string y = str.substr(pos + 1);

				// next line end (absolute or (relative to last one? or absolute?)
				if (mode == 'm') {
					p2.x += atof(x.c_str()) * PIXEL_TO_CM;
					p2.y -= atof(y.c_str()) * PIXEL_TO_CM;
				} else if (mode == 'M') {
					p2.x =                   atof(x.c_str()) * PIXEL_TO_CM;
					p2.y = svgFile->height - atof(y.c_str()) * PIXEL_TO_CM;
				}

				// buffer one point, then start drawing lines
				if (cnt == 0) {++cnt;} else {
					sPath->lines.push_back(Line(p1,p2));
				}

				// previous line end
				p1 = p2;

			}

			// if necessary: transform this path
			applyTransform(path, sPath);

			// next path
			path = path->NextSiblingElement("path");

		}

		// if necessary: transform all elements within this layer
		applyTransform(layer, dst);

		return dst;

	}



}

#endif /* SVGLOADER_H_ */
