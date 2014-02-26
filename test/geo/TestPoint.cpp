/*
 * TestPoint.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: Frank Ebner
 */

#ifdef WITH_TESTS

	#include "../Test.h"
	#include "../../geo/Point.h"

	namespace K {

	TEST(Point, Distance) {

		Point p1(0,0);
		Point p2(100,100);

		EXPECT_FLOAT_EQ(141.42135f, (float) p1.getDistance(p2));
		EXPECT_FLOAT_EQ(141.42135f, (float) p2.getDistance(p1));

	}

}

#endif
