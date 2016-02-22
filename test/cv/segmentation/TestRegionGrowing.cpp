#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/segmentation/RegionGrowing.h"

using namespace K;

TEST(RegionGrowing, grow) {

	ImageChannel img(16, 16);
	img.set(8,8, 1);


	{
		Segment reg;

		// the pixel itself
		reg = RegionGrowing::get(img, Point2i(8,8));	ASSERT_EQ(1, reg.points.size());		ASSERT_EQ(1, reg.avg);

		// all other pixels
		reg = RegionGrowing::get(img, Point2i(7,8));	ASSERT_EQ(256-1, reg.points.size());	ASSERT_EQ(0, reg.avg);

	}

	// slightly bigger region
	img.set(7,8, 1);
	img.set(9,8, 1);
	img.set(8,7, 1);
	img.set(8,9, 1);

	{
		Segment reg;

		// the pixels in the center
		reg = RegionGrowing::get(img, Point2i(8,8));	ASSERT_EQ(5, reg.points.size());		ASSERT_EQ(1, reg.avg);
		reg = RegionGrowing::get(img, Point2i(7,8));	ASSERT_EQ(5, reg.points.size());		ASSERT_EQ(1, reg.avg);
		reg = RegionGrowing::get(img, Point2i(9,8));	ASSERT_EQ(5, reg.points.size());		ASSERT_EQ(1, reg.avg);
		reg = RegionGrowing::get(img, Point2i(8,7));	ASSERT_EQ(5, reg.points.size());		ASSERT_EQ(1, reg.avg);
		reg = RegionGrowing::get(img, Point2i(8,9));	ASSERT_EQ(5, reg.points.size());		ASSERT_EQ(1, reg.avg);

		// all other pixels
		reg = RegionGrowing::get(img, Point2i(0,0));	ASSERT_EQ(256-5, reg.points.size());	ASSERT_EQ(0, reg.avg);

	}

}



#endif


