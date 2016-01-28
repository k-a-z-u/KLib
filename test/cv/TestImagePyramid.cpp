

#ifdef WITH_TESTS
#define WITH_ASSERTIONS

#include "../Test.h"
#include "../../cv/ImagePyramid.h"
#include "../../cv/ImageFactory.h"
#include <sstream>
using namespace K;


#include <fstream>
void plot(int idx, ImageChannel& img) {

	std::ofstream out("/tmp/1" + std::to_string(idx) + ".dat");

	for (int y = 0; y < img.getHeight(); ++y) {
		for (int x = 0; x < img.getWidth(); ++x) {
			out << x << " " << y << " " << img.get(x,y) << std::endl;
		}
		out << std::endl;
	}
	out.close();

}

#include <omp.h>
TEST(ImagePyramid, create) {

	ImageChannel img(2048,2048);

	for (int y = 0; y < img.getHeight(); ++y) {
		for (int x = 0; x < img.getWidth(); ++x) {
			const float v = (((x+y)/8%2) == 0) ? 1.0 : 0.0;
			img.set(x,y,v);
		}
	}

	omp_set_dynamic(0);
	omp_set_num_threads(3);

	ImagePyramid pyr(img);
	for (int i = 0; i < pyr.size(); ++i) {
		std::string file = "/tmp/" + std::to_string(i) + ".png";
#ifdef WITH_PNG
		ImageFactory::writePNG(file, pyr.get(i));
#endif
		//plot(i, pyr.get(i));
	}

}


#endif



