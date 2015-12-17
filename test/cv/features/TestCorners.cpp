


#ifdef WITH_TESTS
#define WITH_ASSERTIONS

#include "../../Test.h"
#include "../../../cv/features/CornerDetectorHarris.h"
#include "../../../cv/features/HOG.h"
#include <sstream>
using namespace K;

#include <fstream>

// TODO: rename
TEST(Corners, HOG) {

	ASSERT_EQ(0, HOG::getBin(8, M_PI*0.0f));
	ASSERT_EQ(2, HOG::getBin(8, M_PI*0.5f));
	ASSERT_EQ(4, HOG::getBin(8, M_PI*1.0f));
	ASSERT_EQ(6, HOG::getBin(8, M_PI*1.5f));
	ASSERT_EQ(0, HOG::getBin(8, M_PI*2.0f));

	ASSERT_EQ(0, HOG::getBin(8, M_PI*2.1f));
	ASSERT_EQ(0, HOG::getBin(8, M_PI*1.9f));
	ASSERT_EQ(0, HOG::getBin(8, M_PI*0.1f));

}

TEST(Corners, Harris) {

	ImageChannel img = ImageFactory::readJPEG("/apps/kiste/KLib/test/cv/features/paint.jpg");

	//ImageFactory::writePNG("/tmp/bla.png", img);
	//ImageChannel img(7,7);
	//img.zero();
	//img.set(3,3, 1.0);


	std::cout << std::fmod(std::atan2(0,1) + 2*M_PI, 2*M_PI)<< std::endl;
	std::cout << std::fmod(std::atan2(1,0)+ 2*M_PI, 2*M_PI) << std::endl;
	std::cout << std::fmod(std::atan2(0,-1)+ 2*M_PI, 2*M_PI) << std::endl;
	std::cout << std::fmod(std::atan2(-1,0) + 2*M_PI, 2*M_PI) << std::endl;

	CornerDetectorHarris cdh(0.002f, 3.0f);
	std::vector<Corner> corners = cdh.getCorners(img);

	HOG hog(8);
	std::ofstream o("/tmp/1.dat");


	for (const Corner& c : corners) {
		Histogram h = hog.get(img, c.x, c.y);
		std::cout << h << std::endl;


		int b = h.getNumBins();
		for (int i = 0; i < b; ++i) {
			float dir = -M_PI + (i * M_PI * 2 / b);
			float val = h.getValue(i);
			int x1 = c.x;
			int y1 = c.y;
			int x2 = x1 + std::cos(dir) * val * 20;
			int y2 = y1 + std::sin(dir) * val * 20;
			o << x1 << " " << -y1 << "\n";
			o << x2 << " " << -y2 << "\n";
			o << "\n";
		}



	}

}



#endif


