#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/features/Outline.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/filter/Threshold.h"

namespace K {

	TEST(Outline, detect) {

		ImageChannel src(5,5);
		src <<	0, 0, 0, 0 ,0,
				0, 1, 1, 1, 0,
				0, 1, 0, 1, 0,
				0, 1, 1, 1, 0,
				0, 0, 0, 0, 0;


		Outline l = OutlineDetection::follow(src, Point2i(1,1), 0.25f);
		ASSERT_EQ(8, l.points.size());

	}



	TEST(Outline, detectImage) {

		ImageChannel src = ImageFactory::readPNG(getDataFile("sudoku_transformed.png"));
		Threshold::inplace(src, 0.7);

		ImageChannel dst(src.getWidth(), src.getHeight());
		Outline l = OutlineDetection::follow(src, Point2i(57,11), 0.25f);
		std::cout << l.points.size() << std::endl;

		for (Point2i p : l.points) {
			src.set(p.x, p.y, 0.5);
		}

		ImageFactory::writePNG("/tmp/bla.png", src);

	}

	TEST(Outline, edges) {

		ImageChannel src = ImageFactory::readPNG(getDataFile("sudoku_transformed.png"));
		Threshold::inplace(src, 0.7);

		ImageChannel dst(src.getWidth(), src.getHeight());
		Outline l = OutlineDetection::follow(src, Point2i(57,11), 0.25f);
		OutlineProcessing::getEdges(l);


	}



}

#endif


