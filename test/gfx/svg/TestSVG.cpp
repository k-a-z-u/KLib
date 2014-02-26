/*
 * TestSVG.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../../Test.h"
#include "../../../fs/File.h"
#include "../../../gfx/svg/SVGFile.h"
#include "../../../gfx/svg/SVGLayer.h"
#include "../../../gfx/svg/SVGPath.h"

using namespace K;

TEST(SVG, load) {


	File folder = File(__FILE__).getParent();
	File f(folder, "1.svg");
	SVGFile file(f);

	SVGComposite* root = file.getLayers();

	std::vector<Line> lines;

	for (SVGElement* e : root->getChilds()) {
		SVGLayer* c = (SVGLayer*) e;
		for (SVGElement* e : c->getChilds()) {
			SVGPath* p = (SVGPath*) e;
			for (const Line& l : p->getLines()) {
				Line l2((int)l.p1.x, (int)l.p1.y, (int)l.p2.x, (int)l.p2.y);
				lines.push_back(l2);
//				std::cout <<
//						"\t(" << l.p1.x << "," << l.p1.y << ")\t" <<
//						"(" << l.p2.x << "," << l.p2.y << ")"
//						" line: " << p->getLineColor().getHex() << std::endl;
			}
		}
	}

	ASSERT_TRUE(Line(5,5,5,125) == lines[0]);
	ASSERT_TRUE(Line(10,120,120,120) == lines[1]);
	ASSERT_TRUE(Line(125,125,125,5) == lines[2]);
	ASSERT_TRUE(Line(10,10,25,25) == lines[3]);
	ASSERT_TRUE(Line(60,60,35,35) == lines[4]);
	ASSERT_TRUE(Line(110,110,110,80) == lines[5]);
	ASSERT_TRUE(Line(110,80,80,80) == lines[6]);
	ASSERT_TRUE(Line(80,80,80,110) == lines[7]);
	ASSERT_TRUE(Line(80,110,100,110) == lines[8]);

}

#endif
