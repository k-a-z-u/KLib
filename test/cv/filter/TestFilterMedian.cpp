
#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/filter/Median.h"

using namespace K;
using namespace K::CV;

TEST(FilterMedian, single) {

	ImageChannel img(3,3);
	img <<
			1,2,3,
			4,5,6,
			7,8,9;

	ASSERT_EQ(5, MedianFilter::get(img, 1,1));

	ASSERT_EQ(6.5, MedianFilter::get(img, 1,2));
	ASSERT_EQ(3.5, MedianFilter::get(img, 1,0));

	ASSERT_EQ(4.5, MedianFilter::get(img, 0,1));
	ASSERT_EQ(5.5, MedianFilter::get(img, 2,1));

}

TEST(FilterMedian, whole) {

	ImageChannel img(3,3);
	img <<
			1,2,3,
			4,5,6,
			7,8,9;

	ImageChannel img2 = MedianFilter::apply(img);

	ASSERT_EQ(5, img2.get(1,1));

	ASSERT_EQ(6.5, img2.get(1,2));
	ASSERT_EQ(3.5, img2.get(1,0));

	ASSERT_EQ(4.5, img2.get(0,1));
	ASSERT_EQ(5.5, img2.get(2,1));

}

#endif
