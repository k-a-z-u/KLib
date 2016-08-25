#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/segmentation/Segmentation.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/filter/Threshold.h"
#include "../../../cv/draw/Drawer.h"

using namespace K;

TEST(Segmentation, segments) {

	ImageChannel img(16, 16);
	img.set(8,8, 1);


	{
		std::vector<Segment<float>> segs = Segmentation::getSegments(img);
		ASSERT_EQ(2, segs.size());
		ASSERT_EQ(256-1, segs[0].points.size());	ASSERT_EQ(0, segs[0].avg);
		ASSERT_EQ(1, segs[1].points.size());		ASSERT_EQ(1, segs[1].avg);
	}

	// slightly bigger region
	img.set(7,8, 1);
	img.set(9,8, 1);
	img.set(8,7, 1);
	img.set(8,9, 1);

	{
		std::vector<Segment<float>> segs = Segmentation::getSegments(img);
		ASSERT_EQ(2, segs.size());
		ASSERT_EQ(256-5, segs[0].points.size());	ASSERT_EQ(0, segs[0].avg);
		ASSERT_EQ(5, segs[1].points.size());		ASSERT_EQ(1, segs[1].avg);
	}

	// 3rd segment
	img.set(15,15,0.5);
	img.set(15,14,0.5);
	img.set(14,15,0.5);

	{
		std::vector<Segment<float>> segs = Segmentation::getSegments(img);
		ASSERT_EQ(3, segs.size());
		ASSERT_EQ(256-5-3, segs[0].points.size());	ASSERT_EQ(0, segs[0].avg);
		ASSERT_EQ(5, segs[1].points.size());		ASSERT_EQ(1, segs[1].avg);
		ASSERT_EQ(3, segs[2].points.size());		ASSERT_EQ(0.5, segs[2].avg);
	}

}

TEST(Segmentation, huge) {

	ImageChannel img(2048, 2048);
	//std::vector<Segment> s = Segmentation::getSegments(img);

}



#endif


