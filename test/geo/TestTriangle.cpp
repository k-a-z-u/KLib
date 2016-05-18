#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Triangle3.h"

namespace K {

	TEST(Triangle3, pointDistance) {

		Triangle3f t(Point3f(0,0,0), Point3f(10,0,0), Point3f(0,10,0));

		Point3f p1(0,0,5);

		ASSERT_EQ(5.0, t.getDistance( Point3f(0,0,5) ));
		ASSERT_EQ(5.0, t.getDistance( Point3f(1,1,5) ));
		ASSERT_EQ(5.0, t.getDistance( Point3f(10,0,5) ));
		ASSERT_EQ(5.0, t.getDistance( Point3f(0,10,5) ));

		ASSERT_NE(INFINITY, t.getDistance( Point3f(5,5,5) ));			// inside
		ASSERT_EQ(INFINITY, t.getDistance( Point3f(5.01f,5.01f,5) ));	// slightly outside


		ASSERT_EQ(INFINITY, t.getDistance( Point3f(0,15,5) ));
		ASSERT_EQ(INFINITY, t.getDistance( Point3f(15,0,5) ));
		ASSERT_EQ(INFINITY, t.getDistance( Point3f(0,-1,5) ));
		ASSERT_EQ(INFINITY, t.getDistance( Point3f(-1,0,5) ));

		ASSERT_EQ(Point3f(1,1,0), t.getIntersection(Point3f(1,1,100)));

	}


}

#endif
