
#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Line.h"

namespace K {

	TEST(Line, length) {

		Line l1(1,0, 0,0);
		EXPECT_EQ(1, l1.getLength());

		Line l2(0,1, 0,0);
		EXPECT_EQ(1, l2.getLength());

		Line l3(0,0, 1,0);
		EXPECT_EQ(1, l3.getLength());

		Line l4(0,0, 0,1);
		EXPECT_EQ(1, l4.getLength());


		Line l5(1,1, 1,1);
		EXPECT_EQ(0, l5.getLength());

		Line l6(0,0, 1,1);
		EXPECT_EQ(std::sqrt(2), l6.getLength());

	}

	/** helper method */
	double getSkalar(Line& l1, Line& l2) {
		Point p1 = l1.p2 - l1.p1;
		Point p2 = l2.p2 - l2.p1;
		return (p1.x * p2.x) + (p1.y * p2.y);
	}


	/** test orthogonality */
	TEST(Line, orthogonal) {

		Line l1(0,0, 1,0);
		Line l2 = l1.getOrthogonal();
		EXPECT_EQ(0, getSkalar(l1,l2));

		Line l3(1,4, 2,-5);
		Line l4 = l3.getOrthogonal();
		EXPECT_EQ(0, getSkalar(l3,l4));

		Line l5(-2,1, -3,9);
		Line l6 = l5.getOrthogonal();
		EXPECT_EQ(0, getSkalar(l5,l6));

		Line l7(1,2, 3,4);
		Point p1(5,-3);
		Line l8 = l7.getOrthogonal(p1);
		EXPECT_EQ(0, getSkalar(l7,l8));
		EXPECT_EQ(p1.x, l8.p1.x);
		EXPECT_EQ(p1.y, l8.p1.y);

	}


}

#endif
