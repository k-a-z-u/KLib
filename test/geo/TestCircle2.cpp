#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Circle2.h"
#include "../../cv/matching/CircleDetection2.h"

#include <random>

namespace K {

	TEST(Circle2, distance) {

		Circle2::GeometricParams circ(Point2i(0,0), 5);

		// center
		ASSERT_EQ(5, circ.getDistanceFromOutline(Point2i(0,0)));

		// on the outline
		ASSERT_EQ(0, circ.getDistanceFromOutline(Point2i(+5,+0)));
		ASSERT_EQ(0, circ.getDistanceFromOutline(Point2i(+0,+5)));
		ASSERT_EQ(0, circ.getDistanceFromOutline(Point2i(-5,+0)));
		ASSERT_EQ(0, circ.getDistanceFromOutline(Point2i(+0,-5)));

		// within the circle
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+4,+0)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+0,+4)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(-4,+0)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+0,-4)));

		// outside of the circle
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+6,+0)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+0,+6)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(-6,+0)));
		ASSERT_EQ(1, circ.getDistanceFromOutline(Point2i(+0,-6)));

	}

	TEST(Circle2, estimateExact) {

		const Circle2::GeometricParams src(Point2f(1, 2), 3);
		const K::Point2f p1 = src.getPointFor(1.0f);
		const K::Point2f p2 = src.getPointFor(1.5f);
		const K::Point2f p3 = src.getPointFor(2.5f);
		const K::Point2f p4 = src.getPointFor(3.5f);
		const K::Point2f p5 = src.getPointFor(4.5f);
		const K::Point2f p6 = src.getPointFor(5.5f);

		const std::vector<K::Point2f> lst = {p1, p2, p3, p4, p5, p6};

		Circle2::CanonicalParams canon = CircleEstimator2<float>::getParams(lst);

		const float rad = canon.getRadius();
		Point2f center = canon.getCenter();
		Circle2::GeometricParams circ = canon.toGeometric();

		std::cout << center.x << " " << center.y << " " << rad << std::endl;

		ASSERT_NEAR(1.0f, center.x, 0.01);
		ASSERT_NEAR(2.0f, center.y, 0.01);
		ASSERT_NEAR(3.0f, rad, 0.01);

		ASSERT_NEAR(1.0f, circ.center.x, 0.01);
		ASSERT_NEAR(2.0f, circ.center.y, 0.01);
		ASSERT_NEAR(3.0f, circ.radius, 0.01);

	}

	TEST(Circle2, estimateNoisy) {

		std::minstd_rand gen;
		std::uniform_real_distribution<float> dist(-0.2f, +0.2f);

		const Circle2::GeometricParams src(Point2f(1, 2), 3);
		const K::Point2f p1 = src.getPointFor(0.0f) + K::Point2f(dist(gen), dist(gen));
		const K::Point2f p2 = src.getPointFor(1.0f) + K::Point2f(dist(gen), dist(gen));
		const K::Point2f p3 = src.getPointFor(2.0f) + K::Point2f(dist(gen), dist(gen));
		const K::Point2f p4 = src.getPointFor(3.0f) + K::Point2f(dist(gen), dist(gen));
		const K::Point2f p5 = src.getPointFor(4.0f) + K::Point2f(dist(gen), dist(gen));
		const K::Point2f p6 = src.getPointFor(5.0f) + K::Point2f(dist(gen), dist(gen));

		const std::vector<K::Point2f> lst = {p1, p2, p3, p4, p5, p6};

		Circle2::CanonicalParams canon = CircleEstimator2<float>::getParams(lst);

		const float rad = canon.getRadius();
		Point2f center = canon.getCenter();
		Circle2::GeometricParams circ = canon.toGeometric();

		std::cout << center.x << " " << center.y << " " << rad << std::endl;

		ASSERT_NEAR(1.0f, center.x, 0.1);
		ASSERT_NEAR(2.0f, center.y, 0.1);
		ASSERT_NEAR(3.0f, rad, 0.1);

		ASSERT_NEAR(1.0f, circ.center.x, 0.1);
		ASSERT_NEAR(2.0f, circ.center.y, 0.1);
		ASSERT_NEAR(3.0f, circ.radius, 0.1);

	}

}


#endif
