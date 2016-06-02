#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/BBox2.h"

namespace K {

	TEST(BBox2, center) {

		BBox2<float> bbox;
		bbox.add(Point2f(10,10));
		bbox.add(Point2f(20,20));
		bbox.center();

		ASSERT_EQ(Point2f(0,0), bbox.getCenter());
		ASSERT_EQ(Point2f(-5,-5), bbox.getMin());
		ASSERT_EQ(Point2f(+5,+5), bbox.getMax());

	}

	TEST(BBox2, move) {

		BBox2<float> bbox;
		bbox.add(Point2f(-5,-5));
		bbox.add(Point2f(+5,+5));
		bbox.moveBy(Point2f(20,20));

		ASSERT_EQ(Point2f(15,15), bbox.getMin());
		ASSERT_EQ(Point2f(25,25), bbox.getMax());

	}

	TEST(BBox2, contains) {

		BBox2<float> bbox;
		bbox.add(Point2f(10,11));
		bbox.add(Point2f(20,21));

		ASSERT_TRUE(bbox.contains(Point2f(10,11)));
		ASSERT_TRUE(bbox.contains(Point2f(20,21)));

		ASSERT_FALSE(bbox.contains(Point2f(10,10)));
		ASSERT_FALSE(bbox.contains(Point2f( 9,10)));
		ASSERT_FALSE(bbox.contains(Point2f(20,22)));
		ASSERT_FALSE(bbox.contains(Point2f(21,21)));

	}

}


#endif
