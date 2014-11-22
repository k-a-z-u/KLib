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

	TEST(Point, sub) {

		Point p1(1,2);
		Point p2(3,6);

		Point p3 = p1-p2;
		EXPECT_EQ(-2, p3.x);
		EXPECT_EQ(-4, p3.y);

		Point p4 = p2-p1;
		EXPECT_EQ(2, p4.x);
		EXPECT_EQ(4, p4.y);

		p2 -= p1;
		EXPECT_EQ(2, p2.x);
		EXPECT_EQ(4, p2.y);

	}

	TEST(Point, add) {

		Point p1(1,2);
		Point p2(3,6);

		Point p3 = p1+p2;
		EXPECT_EQ(4, p3.x);
		EXPECT_EQ(8, p3.y);

		Point p4 = p2+p1;
		EXPECT_EQ(4, p4.x);
		EXPECT_EQ(8, p4.y);

		p2 += p1;
		EXPECT_EQ(4, p2.x);
		EXPECT_EQ(8, p2.y);

	}

}

#endif
