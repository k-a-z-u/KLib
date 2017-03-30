
#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/filter/Rotate.h"

using namespace K;
using namespace K::CV;

TEST(FilterRotate, deg90) {

	K::Point2f center(1,1);
	K::ImageChannel img(3,3);
	img.zero();
	img.set(1,0, 1.0);
	img.set(1,1, 1.0);
	img.set(1,2, 1.0);

	K::ImageChannel img2 = K::CV::Rotate::apply<K::CV::Interpolation::None>(img, center, M_PI/2);

	ASSERT_EQ(1.0, img2.get(0,1));
	ASSERT_EQ(1.0, img2.get(1,1));
	ASSERT_EQ(1.0, img2.get(2,1));

}


#endif
