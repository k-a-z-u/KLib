#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/features/HOG2.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"


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

	// http://scikit-image.org/docs/dev/_images/plot_hog_1.png

	//K::ImageChannel img = ImageFactory::readJPEG(getDataFile("cameraman.jpg"));
	//K::ImageChannel img = ImageFactory::readJPEG(getDataFile("astronaut.jpg"));
	K::ImageChannel img = ImageFactory::readPNG(getDataFile("blur.png"));

	const int numBins = 9;

	K::HOG2 hog(img, 4, numBins);

	K::ImageChannel out(img.getWidth()*2, img.getHeight()*2);
	K::Drawer d(out);
	d.setForeground(1);

	const int bs = 8;
	for (int x = bs/2; x < img.getWidth()-bs; x += bs) {
		for (int y = bs/2; y < img.getHeight()-bs; y += bs) {

			const std::vector<float> hist = hog.getHistogram(x,y);
			if (hist.empty()) {
				continue;
			}

			for (int i = 0; i < hist.size(); ++i) {
				const float rad = i * M_PI / numBins;// + M_PI_2;
				const float mag = hist[i];
				const float x1 = x + std::cos(rad) * 6 * mag;
				const float y1 = y + std::sin(rad) * 6 * mag;
				d.drawLine(x*2,y*2, x1*2,y1*2);
				const float x2 = x + std::cos(rad+M_PI) * 6 * mag;
				const float y2 = y + std::sin(rad+M_PI) * 6 * mag;
				d.drawLine(x*2,y*2, x2*2,y2*2);
			}

		}
	}

	K::ImageFactory::writePNG("/tmp/hog1.png", out);
	int i = 0; (void) i;


}

#endif
