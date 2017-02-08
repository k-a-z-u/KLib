#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/features/HOG2.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"
#include "../../../cv/filter/Resize.h"
#include "../../../cv/filter/Normalize.h"



using namespace K;

TEST(HOG2, benchmark) {

	K::ImageChannel img(512, 512);

	K::HOG2 hog(img, K::Size2i(8,8), 9, K::Size2i(16,16));

}

TEST(H__OG2, resultSize) {

//	struct Setting {
//		int bins;
//		K::Size2i cellSize;
//		K::Size2i blockSize;
//		K::Size2i blockStride;
//	};
	K::ImageChannel img(64, 128);


	{
		K::HOG2 hog(img, K::Size2i(8,8), 9, K::Size2i(16,16));
		ASSERT_EQ(3780, hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(8,8)).size());
	}

	{
		K::HOG2 hog(img, K::Size2i(8,8), 9, K::Size2i(8,8));
		ASSERT_EQ(1152, hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(8,8)).size());
	}

	{
		K::HOG2 hog(img, K::Size2i(8,8), 8, K::Size2i(8,8));
		ASSERT_EQ(1024, hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(8,8)).size());
	}

	{
		K::HOG2 hog(img, K::Size2i(8,8), 8, K::Size2i(8,8));
		ASSERT_EQ(15*31*8, hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(4,4)).size());
	}

	{
		K::HOG2 hog(img, K::Size2i(8,8), 8, K::Size2i(8,8));
		ASSERT_EQ((64-7)*(128-7)*8, hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(1,1)).size());
	}



	// 24x24 block
	{
		K::HOG2 hog(img, K::Size2i(8,8), 9, K::Size2i(24,24));
		ASSERT_EQ((3*3*9)*(1+(64-24)/8)*(1+(128-24)/8), hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(8,8)).size());
	}

	// 24x24 block
	{
		K::HOG2 hog(img, K::Size2i(8,8), 9, K::Size2i(24,24));
		ASSERT_EQ((3*3*9)*(1+(64-24)/4)*(1+(128-24)/4), hog.getFeature(K::Point2i(32,64), K::Size2i(64,128), K::Size2i(4,4)).size());
	}

}

TEST(HOG2, Area) {

	K::HOG2::Area a1(K::Point2i(1,1), K::Size2i(2,2), K::Size2i(2,2), K::Size2i(1,1));
	ASSERT_EQ(0, a1.sx);	ASSERT_EQ(0, a1.sy);
	ASSERT_EQ(1, a1.wx);	ASSERT_EQ(1, a1.wy);
	ASSERT_EQ(1, a1.getBlockCenter(0,0).x);
	ASSERT_EQ(1, a1.getBlockCenter(0,0).y);

	K::HOG2::Area a2(K::Point2i(0,0), K::Size2i(2,2), K::Size2i(2,2), K::Size2i(1,1));
	ASSERT_EQ(-1, a2.sx);	ASSERT_EQ(-1, a2.sy);
	ASSERT_EQ( 1, a2.wx);	ASSERT_EQ( 1, a2.wy);
	ASSERT_EQ( 0, a2.getBlockCenter(0,0).x);
	ASSERT_EQ( 0, a2.getBlockCenter(0,0).y);

	K::HOG2::Area a3(K::Point2i(0,0), K::Size2i(2,2), K::Size2i(3,3), K::Size2i(1,1));
	ASSERT_EQ(-1, a3.sx);	ASSERT_EQ(-1, a3.sy);	// rounded down
	ASSERT_EQ( 2, a3.wx);	ASSERT_EQ( 2, a3.wy);	// shift window 1 time

	K::HOG2::Area a4(K::Point2i(0,0), K::Size2i(2,2), K::Size2i(4,4), K::Size2i(1,1));
	ASSERT_EQ(-2, a4.sx);	ASSERT_EQ(-2, a4.sy);	// one window left/right of (0,0) one window above/below (0,0)
	ASSERT_EQ(-1, a4.cx);	ASSERT_EQ(-1, a4.cy);
	ASSERT_EQ( 3, a4.wx);	ASSERT_EQ( 3, a4.wy);	// shift window 2 times

	K::HOG2::Area a5(K::Point2i(0,0), K::Size2i(2,2), K::Size2i(6,6), K::Size2i(1,1));
	ASSERT_EQ(-3, a5.sx);	ASSERT_EQ(-3, a5.sy);	// 1,5 window left/right of (0,0) 1,5 window above/below (0,0)
	ASSERT_EQ(-2, a5.cx);	ASSERT_EQ(-2, a5.cy);
	ASSERT_EQ( 5, a5.wx);	ASSERT_EQ( 5, a5.wy);	// shift window 4 times

}


TEST(HOG2, facts1) {

	K::ImageChannel img;

	const K::Size2i cellSize(8,8);
	const K::Size2i blockSize(16,16);
	const int numBins = 9;

	K::HOG2 hog(img, cellSize, numBins, blockSize);

	//ASSERT_EQ(20, hog.degPerBin());

	ASSERT_EQ(0, hog.degToBin(0));
	ASSERT_EQ(1, hog.degToBin(20));
	ASSERT_EQ(2, hog.degToBin(40));
	ASSERT_EQ(3, hog.degToBin(60));
	ASSERT_EQ(3.5, hog.degToBin(70));

	//ASSERT_EQ(20, hog.degPerBin());

	{
		K::HOG2::Contributions c = hog.getContribution(0.0, 1.0);
		ASSERT_EQ(c.c1.bin, 0);		ASSERT_EQ(c.c1.weight, 1);
		ASSERT_EQ(c.c2.bin, 0);		ASSERT_EQ(c.c2.weight, 0);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(10.0, 1.0);
		ASSERT_EQ(c.c1.bin, 0);		ASSERT_EQ(c.c1.weight, 0.5);
		ASSERT_EQ(c.c2.bin, 1);		ASSERT_EQ(c.c2.weight, 0.5);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(20.0, 1.0);
		ASSERT_EQ(c.c1.bin, 1);		ASSERT_EQ(c.c1.weight, 1);
		ASSERT_EQ(c.c2.bin, 1);		ASSERT_EQ(c.c2.weight, 0);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(30.0, 1.0);
		ASSERT_EQ(c.c1.bin, 1);		ASSERT_EQ(c.c1.weight, 0.5);
		ASSERT_EQ(c.c2.bin, 2);		ASSERT_EQ(c.c2.weight, 0.5);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(160.0, 1.0);
		ASSERT_EQ(c.c1.bin, 8);		ASSERT_EQ(c.c1.weight, 1);
		ASSERT_EQ(c.c2.bin, 8);		ASSERT_EQ(c.c2.weight, 0);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(170.0, 1.0);
		ASSERT_EQ(c.c1.bin, 8);		ASSERT_EQ(c.c1.weight, 0.5);
		ASSERT_EQ(c.c2.bin, 0);		ASSERT_EQ(c.c2.weight, 0.5);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(180.0, 1.0);
		ASSERT_EQ(c.c1.bin, 0);		ASSERT_EQ(c.c1.weight, 1);
		ASSERT_EQ(c.c2.bin, 0);		ASSERT_EQ(c.c2.weight, 0);
	}
	{
		K::HOG2::Contributions c = hog.getContribution(200.0, 1.0);
		ASSERT_EQ(c.c1.bin, 1);		ASSERT_EQ(c.c1.weight, 1);
		ASSERT_EQ(c.c2.bin, 1);		ASSERT_EQ(c.c2.weight, 0);
	}

}

TEST(HOG2, pattern) {

	K::ImageChannel img(8,8);

	{
		K::Size2i bs(8,8);
		K::Size2i cs(8,8);
		K::HOG2 hog(img, cs, 9, bs);
		std::vector<K::HOG2::CellPoint> pts = hog.getCellPoints(K::HOG2::RECTANGULAR);
		ASSERT_EQ(8*8, pts.size());
		ASSERT_EQ(pts[0].impact, pts[7].impact); // symmetric
	}

	{
		K::Size2i bs(4,4);
		K::Size2i cs(4,4);
		K::HOG2 hog(img, cs, 9, bs);
		std::vector<K::HOG2::CellPoint> pts = hog.getCellPoints(K::HOG2::RECTANGULAR);
		ASSERT_EQ(4*4, pts.size());
		ASSERT_EQ(pts[0].impact, pts[3].impact); // symmetric
	}

	{
		K::Size2i bs(3,3);
		K::Size2i cs(3,3);
		K::HOG2 hog(img, cs, 9, bs);
		std::vector<K::HOG2::CellPoint> pts = hog.getCellPoints(K::HOG2::RECTANGULAR);
		ASSERT_EQ(3*3, pts.size());
		ASSERT_EQ(pts[0].impact, pts[2].impact); // symmetric
	}

}

TEST(HOG2, blackWhiteCellHorizontal) {

	K::ImageChannel img = ImageFactory::readPNG(getDataFile("bw_h.png"));

	K::Size2i bs(6,6);
	K::Size2i cs(6,6);

	K::HOG2 hog(img, cs, 9, bs);

	K::HOG2::Vector vec1 = hog.getCell( 4, 4);
	ASSERT_EQ(0, vec1.length());

	// only the first value is set [0 degree bin = to the right/left]
	K::HOG2::Vector vec2;
	vec2 = hog.getCell( 5, 4);	ASSERT_NE(0, vec2.length());	ASSERT_EQ(vec2.length(), vec2[0]);	// ~ 2
	vec2 = hog.getCell( 8, 4);	ASSERT_NE(0, vec2.length());	ASSERT_EQ(vec2.length(), vec2[0]);	// ~ 5
	vec2 = hog.getCell(11, 4);	ASSERT_NE(0, vec2.length());	ASSERT_EQ(vec2.length(), vec2[0]);	// ~ 2

	K::HOG2::Vector vec3 = hog.getCell(12, 4);
	ASSERT_EQ(0, vec3.length());

	int i = 0; (void) i;

}

TEST(HOG2, blackWhiteCellVertical) {

	K::ImageChannel img = ImageFactory::readPNG(getDataFile("bw_v.png"));

	K::Size2i bs(6,6);
	K::Size2i cs(6,6);

	K::HOG2 hog(img, cs, 9, bs);

	K::HOG2::Vector vec1 = hog.getCell( 4, 4);
	ASSERT_EQ(0, vec1.length());

	// 90  degree is equally split among bins 4 and 5
	K::HOG2::Vector vec2;
	vec2 = hog.getCell( 4,  5);	ASSERT_NE(0, vec2.length());	ASSERT_NEAR(vec2[4], vec2[5], 0.0001);
	vec2 = hog.getCell( 4,  8);	ASSERT_NE(0, vec2.length());	ASSERT_NEAR(vec2[4], vec2[5], 0.0001);
	vec2 = hog.getCell( 4, 11);	ASSERT_NE(0, vec2.length());	ASSERT_NEAR(vec2[4], vec2[5], 0.0001);

	K::HOG2::Vector vec3 = hog.getCell(4, 12);
	ASSERT_EQ(0, vec3.length());

	int i = 0; (void) i;

}

static int numNonZero(const K::HOG2::Vector& v) {
	int cnt = 0;
	for (float f : v) {
		if (f > 0) {++cnt;}
	}
	return cnt;
}

TEST(HxOG2, blackWhiteBlockHorizontal) {

	K::ImageChannel img = ImageFactory::readPNG(getDataFile("bw_h.png"));

	K::Size2i bs(6,6);
	K::Size2i cs(3,3);

	K::HOG2 hog(img, cs, 9, bs);

	K::HOG2::Vector vec1 = hog.getBlock(4, 4);
	ASSERT_EQ(0, vec1.length());

	// only the first value is set [0 degree bin = to the right/left]
	K::HOG2::Vector vec2;
	vec2 = hog.getBlock( 5, 4);	ASSERT_NE(0, vec2.length());		ASSERT_EQ(2, numNonZero(vec2));		// edge contributes to 2 of 4 blocks
	vec2 = hog.getBlock( 8, 4);	ASSERT_NE(0, vec2.length());		ASSERT_EQ(4, numNonZero(vec2));		// edge contributes to 4 of 4 blocks
	vec2 = hog.getBlock(11, 4);	ASSERT_NE(0, vec2.length());		ASSERT_EQ(2, numNonZero(vec2));		// edge contributes to 2 of 4 blocks

	K::HOG2::Vector vec3 = hog.getBlock(12, 4);
	ASSERT_EQ(0, vec3.length());

	int i = 0; (void) i;

}


TEST(HOG2, detect) {

	K::ImageChannel imgPattern = ImageFactory::readPNG(getDataFile("bolt.png"));
	K::ImageChannel imgSearch = ImageFactory::readJPEG(getDataFile("bolt_full.jpg"));

	imgSearch = K::CV::Resize::apply<K::CV::Interpolation::Bilinear>(imgSearch, 0.45f);		// same size

	K::Size2i cs(8,8);
	K::Size2i bs(8,8);
	const int numBins = 9;
	const int sw = 64;
	const int sh = 128;
	K::HOG2 hogPattern(imgPattern, cs, numBins, bs);
	K::HOG2 hogSearch(imgSearch, cs, numBins, bs);

	const HOG2::Vector featPattern = hogPattern.getFeature(K::Point2i(sw/2, sh/2), K::Size2i(sw,sh));

	K::ImageChannel out(imgSearch.getWidth(), imgSearch.getHeight());

	const int stride = 1;
	for (int x = sw/2; x <= imgSearch.getWidth()-sw/2; x += stride) {
		for (int y = sh/2; y <= imgSearch.getHeight()-sh/2; y += stride) {

			const HOG2::Vector featSearch = hogSearch.getFeature(K::Point2i(x,y),  K::Size2i(sw,sh));

			const float diff = featSearch.distance(featPattern);
			out.set(x,y,diff);

		}
	}

	K::CV::Normalize::inplace(out);
	K::ImageFactory::writePNG("/tmp/hogDiff.png", out);

}

/*
TEST(HOG2, build) {

	// http://scikit-image.org/docs/dev/_images/plot_hog_1.png

	//K::ImageChannel img = ImageFactory::readJPEG(getDataFile("cameraman.jpg"));
	//K::ImageChannel img = ImageFactory::readJPEG(getDataFile("astronaut.jpg"));
	//K::ImageChannel img = ImageFactory::readPNG(getDataFile("blur.png"));
	K::ImageChannel img = ImageFactory::readPNG(getDataFile("bolt.png"));

	const int bs = 8;
	const int numBins = 9;

	K::HOG2 hog(img, 8, numBins);

	// make bigger for debug-view
	const int s = 5;

	//K::ImageChannel out(img.getWidth()*s, img.getHeight()*s);

	K::ImageChannel out = K::CV::Resize::apply<K::CV::Interpolation::None>(img, s);

	K::Drawer d(out);
	d.setForeground(1);


	const HOG2::Vector feat = hog.getFeature(32,64,  64,128);
	ASSERT_EQ(3780, feat.size());

	for (int x = bs/2; x <= img.getWidth()-bs/2; x += bs) {
		for (int y = bs/2; y <= img.getHeight()-bs/2; y += bs) {

			HOG2::Vector hist = hog.getBlock(x,y);

			if (hist.empty()) {
				continue;
			}

			hist.normalize();

			const int len = 8;
			for (int i = 0; i < hist.size(); ++i) {
				const float rad = i * M_PI / numBins + M_PI_2;
				const float mag = hist[i];
				const float x1 = x + std::cos(rad) * len * mag;
				const float y1 = y + std::sin(rad) * len * mag;
				d.drawLine(x*s,y*s, x1*s,y1*s);
				const float x2 = x + std::cos(rad+M_PI) * len * mag;
				const float y2 = y + std::sin(rad+M_PI) * len * mag;
				d.drawLine(x*s,y*s, x2*s,y2*s);
			}

		}
	}

	K::ImageFactory::writePNG("/tmp/hog2.png", out);
	int i = 0; (void) i;


}
*/

#endif
