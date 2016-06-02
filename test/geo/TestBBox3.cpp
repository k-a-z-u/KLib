#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/BBox3.h"

namespace K {

	TEST(BBox3, center) {

		BBox3<float> bbox;
		bbox.add(Point3f(10,12,14));
		bbox.add(Point3f(20,24,28));
		bbox.center();

		ASSERT_EQ(Point3f(0,0,0), bbox.getCenter());
		ASSERT_EQ(Point3f(-5,-6,-7), bbox.getMin());
		ASSERT_EQ(Point3f(+5,+6,+7), bbox.getMax());

	}

	TEST(BBox3, move) {

		BBox3<float> bbox;
		bbox.add(Point3f(-5,-5,-5));
		bbox.add(Point3f(+5,+5,+5));
		bbox.moveBy(Point3f(20,21,22));

		ASSERT_EQ(Point3f(15,16,17), bbox.getMin());
		ASSERT_EQ(Point3f(25,26,27), bbox.getMax());

	}

	TEST(BBox3, contains) {

		BBox3<float> bbox;
		bbox.add(Point3f(10,11,12));
		bbox.add(Point3f(20,21,22));

		ASSERT_TRUE(bbox.contains(Point3f(10,11,12)));
		ASSERT_TRUE(bbox.contains(Point3f(20,21,22)));

		ASSERT_FALSE(bbox.contains(Point3f(9,11,12)));
		ASSERT_FALSE(bbox.contains(Point3f(10,10,12)));
		ASSERT_FALSE(bbox.contains(Point3f(10,11,11)));

		ASSERT_FALSE(bbox.contains(Point3f(21,21,22)));
		ASSERT_FALSE(bbox.contains(Point3f(20,22,22)));
		ASSERT_FALSE(bbox.contains(Point3f(20,21,23)));


	}

}


#endif
