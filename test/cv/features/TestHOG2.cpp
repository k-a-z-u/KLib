#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/features/HOG2.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"
#include "../../../cv/filter/Resize.h"
#include "../../../cv/filter/Normalize.h"



using namespace K;

TEST(HOG2, facts1) {

	K::ImageChannel img;

	const int numBins = 9;
	K::HOG2 hog(img, 4, numBins);

	ASSERT_EQ(20, hog.degPerBin());

	ASSERT_EQ(0, hog.degToBin(0));
	ASSERT_EQ(1, hog.degToBin(20));
	ASSERT_EQ(2, hog.degToBin(40));
	ASSERT_EQ(3, hog.degToBin(60));
	ASSERT_EQ(3.5, hog.degToBin(70));

	ASSERT_EQ(20, hog.degPerBin());

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

TEST(HOG2, detect) {

	K::ImageChannel imgPattern = ImageFactory::readPNG(getDataFile("bolt.png"));
	K::ImageChannel imgSearch = ImageFactory::readJPEG(getDataFile("bolt_full.jpg"));

	imgSearch = K::CV::Resize::apply<K::CV::Interpolation::Bilinear>(imgSearch, 0.45);		// same size

	const int bs = 8;
	const int numBins = 9;
	const int sw = 64;
	const int sh = 128;
	K::HOG2 hogPattern(imgPattern, 8, numBins);
	K::HOG2 hogSearch(imgSearch, 8, numBins);

	const HOG2::Vector featPattern = hogPattern.getFeature(sw/2,sh/2,  sw,sh);

	K::ImageChannel out(imgSearch.getWidth(), imgSearch.getHeight());

	const int stride = 1;
	for (int x = sw/2; x <= imgSearch.getWidth()-sw/2; x += stride) {
		for (int y = sh/2; y <= imgSearch.getHeight()-sh/2; y += stride) {

			const HOG2::Vector featSearch = hogSearch.getFeature(x,y,  sw,sh);

			const float diff = featSearch.distance(featPattern);
			out.set(x,y,diff);

		}
	}

	K::CV::Normalize::inplace(out);
	K::ImageFactory::writePNG("/tmp/hogDiff.png", out);

}

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

#endif
