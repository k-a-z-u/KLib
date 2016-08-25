#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/Homography.h"
#include "../../../cv/ImageFactory.h"

namespace K {

	TEST(Homography, estimate) {

		Homography h;
		h.addCorrespondence(  0,  0,	58,12);
		h.addCorrespondence(200,  0,	162,15);
		h.addCorrespondence(200,200,	154,187);
		h.addCorrespondence(  0,200,	9,160);
		h.estimate();

		float x,y;
		h.worldToImg( 58, 12,	x,y);		ASSERT_NEAR(  0, x, 0.01);	ASSERT_NEAR(  0, y, 0.01);
		h.worldToImg(162, 15,	x,y);		ASSERT_NEAR(200, x, 0.01);	ASSERT_NEAR(  0, y, 0.01);
		h.worldToImg(154,187,	x,y);		ASSERT_NEAR(200, x, 0.01);	ASSERT_NEAR(200, y, 0.01);
		h.worldToImg(  9,160,	x,y);		ASSERT_NEAR(  0, x, 0.01);	ASSERT_NEAR(200, y, 0.01);

		h.imgToWorld(  0,  0,	x,y);		ASSERT_NEAR( 58, x, 0.01);	ASSERT_NEAR( 12, y, 0.01);
		h.imgToWorld(200,  0,	x,y);		ASSERT_NEAR(162, x, 0.01);	ASSERT_NEAR( 15, y, 0.01);
		h.imgToWorld(200,200,	x,y);		ASSERT_NEAR(154, x, 0.01);	ASSERT_NEAR(187, y, 0.01);
		h.imgToWorld(  0,200,	x,y);		ASSERT_NEAR(  9, x, 0.01);	ASSERT_NEAR(160, y, 0.01);

	}

	TEST(Homography, transform) {

		const ImageChannel src = ImageFactory::readPNG(getDataFile("sudoku_transformed.png"));
		ImageChannel dst(src.getWidth(), src.getHeight());

		Homography h;
		h.addCorrespondence(  0,  0,	58,12);
		h.addCorrespondence(200,  0,	162,15);
		h.addCorrespondence(200,200,	154,187);
		h.addCorrespondence(  0,200,	9,160);
		h.estimate();

		h.undo(src, dst);
		ImageFactory::writePNG(getTempFile("/sudoku_untransformed.png"), dst);

	}

	TEST(Homography, values) {

		Homography h;
		h.addCorrespondence(0, 0,		0, 0);
		h.addCorrespondence(1, 0,		2, 0);
		h.addCorrespondence(1, 1,		2, 2);
		h.addCorrespondence(0, 1,		0, 2);

		//h.addCorrespondence(0, 2,		0, 4);

		h.estimate();

		const float d = 0.00001;
		float wx, wy;
		h.imgToWorld(0, 0, wx, wy);	ASSERT_NEAR(0, wx, d); ASSERT_NEAR(0, wy, d);
		h.imgToWorld(1, 0, wx, wy);	ASSERT_NEAR(2, wx, d); ASSERT_NEAR(0, wy, d);
		h.imgToWorld(1, 1, wx, wy);	ASSERT_NEAR(2, wx, d); ASSERT_NEAR(2, wy, d);
		h.imgToWorld(0, 1, wx, wy);	ASSERT_NEAR(0, wx, d); ASSERT_NEAR(2, wy, d);

		float ix, iy;
		h.worldToImg(0, 0, ix, iy);	ASSERT_NEAR(0, ix, d); ASSERT_NEAR(0, iy, d);
		h.worldToImg(2, 0, ix, iy);	ASSERT_NEAR(1, ix, d); ASSERT_NEAR(0, iy, d);
		h.worldToImg(2, 2, ix, iy);	ASSERT_NEAR(1, ix, d); ASSERT_NEAR(1, iy, d);
		h.worldToImg(0, 2, ix, iy);	ASSERT_NEAR(0, ix, d); ASSERT_NEAR(1, iy, d);




	}

}

#endif
