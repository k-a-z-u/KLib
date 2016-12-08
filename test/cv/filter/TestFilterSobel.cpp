#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/filter/Sobel.h"
#include "../../../cv/ImageFactory.h"

namespace K {

	TEST(FilterSobel, single) {

		K::ImageChannel img = K::ImageFactory::readPNG(getDataFile("lens_1.png"));

		K::ImageChannel out = K::CV::Sobel::apply(img);

		K::ImageFactory::writePNG("/tmp/sobel.png", out);


	}



}




#endif
