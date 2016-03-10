
#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/filter/Threshold.h"

using namespace K;

TEST(FilterThreshold, normal) {

	ImageChannel img(2,2);
	img.set(0,0,0.1f);
	img.set(1,0,0.5f);
	img.set(1,1,0.6f);
	img.set(0,1,0.7f);

	Threshold::inplace(img, 0.5f);
	ASSERT_EQ(0, img.get(0,0));
	ASSERT_EQ(0, img.get(1,0));
	ASSERT_EQ(1, img.get(1,1));
	ASSERT_EQ(1, img.get(0,1));

}

TEST(FilterThreshold, bitmap) {

	ImageChannel img(2,2);
	img.set(0,0,0.1f);
	img.set(1,0,0.5f);
	img.set(1,1,0.6f);
	img.set(0,1,0.7f);

	Bitmap b1 = Threshold::bitmap(img, 0.5f);
	ASSERT_EQ(false, b1.isSet(0,0));
	ASSERT_EQ(false, b1.isSet(1,0));
	ASSERT_EQ(true, b1.isSet(1,1));
	ASSERT_EQ(true, b1.isSet(0,1));

	Bitmap b2 = Threshold::bitmap(img, 0.5f, true);
	ASSERT_EQ(true, b2.isSet(0,0));
	ASSERT_EQ(true, b2.isSet(1,0));
	ASSERT_EQ(false, b2.isSet(1,1));
	ASSERT_EQ(false, b2.isSet(0,1));

}


#endif
