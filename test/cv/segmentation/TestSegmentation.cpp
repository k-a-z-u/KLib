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
		std::vector<Segment> segs = Segmentation::getSegments(img);
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
		std::vector<Segment> segs = Segmentation::getSegments(img);
		ASSERT_EQ(2, segs.size());
		ASSERT_EQ(256-5, segs[0].points.size());	ASSERT_EQ(0, segs[0].avg);
		ASSERT_EQ(5, segs[1].points.size());		ASSERT_EQ(1, segs[1].avg);
	}

	// 3rd segment
	img.set(15,15,0.5);
	img.set(15,14,0.5);
	img.set(14,15,0.5);

	{
		std::vector<Segment> segs = Segmentation::getSegments(img);
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

TEST(Segmentation, text) {

	ImageChannel img = ImageFactory::readJPEG(getDataFile("text_scan.jpg"));
	Threshold::inplace(img, 0.80f);

	ImageFactory::writePNG("/tmp/bla.png", img);

	Bitmap used(img);
	std::vector<Segment> segs = Segmentation::getSegments(img, used, 0.5f);

	int cnt = 0;
	for (Segment& s : segs) {
		if (++cnt > 128) {break;}
		ImageChannel i = s.asImage(false);
		ImageFactory::writePNG("/tmp/" + std::to_string(cnt) + ".png", i);
	}

	ImageChannel img2(img.getWidth(), img.getHeight());
	img2.ones();
	Drawer d(img2);
	for (Segment& s : segs) {
		const BBox2i bb = s.calcBBox();
		d.drawRect(bb.getMin(), bb.getMax());
	}
	ImageFactory::writePNG("/tmp/segs.png", img2);


}


#endif


