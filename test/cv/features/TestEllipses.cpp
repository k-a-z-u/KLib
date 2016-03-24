#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/features/Ellipse.h"
#include "../../../cv/ImageFactory.h"


using namespace K;


TEST(Ellipses, detect) {


	ImageChannel img = ImageFactory::readPNG(getDataFile("ellipses.png"));

	Ellipses el;
	std::vector<Ellipse> lst = el.detect(img);

	int i = 0; (void) i;

}

#endif
