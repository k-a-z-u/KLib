#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Plane3.h"
#include "../../cv/matching/PlaneDetection3.h"

#include <random>

namespace K {


	TEST(Plane3, matchingMany) {

		std::minstd_rand gen;
		std::uniform_real_distribution<float> dist(-10, +10);

		std::vector<Point3f> pts;
		for (int i = 0; i < 10; ++i) {
			Point3f p(dist(gen), dist(gen), 3);
			pts.push_back(p);
		}

		const Plane3f::CoordinateParams params = PlaneEstimator3<float>::getParams(pts);
		ASSERT_NEAR(3, params.distance, 0.01);
		ASSERT_NEAR(0, params.normal.x, 0.01);
		ASSERT_NEAR(0, params.normal.y, 0.01);
		ASSERT_NEAR(1, std::abs(params.normal.z), 0.01);

	}

	TEST(Plane3, matching3X) {

		std::vector<Point3f> pts = {
			Point3f(0,0,0),
			Point3f(0,1,0),
			Point3f(0,0,1)
		};

		const Plane3f::CoordinateParams params = PlaneEstimator3<float>::getParams(pts);
		ASSERT_NEAR(0, params.distance, 0.01);
		ASSERT_NEAR(1, std::abs(params.normal.x), 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.y), 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.z), 0.01);

	}

	TEST(Plane3, matching3Y) {

		std::vector<Point3f> pts = {
			Point3f(0,0,0),
			Point3f(1,0,0),
			Point3f(0,0,1)
		};

		const Plane3f::CoordinateParams params = PlaneEstimator3<float>::getParams(pts);
		ASSERT_NEAR(0, params.distance, 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.x), 0.01);
		ASSERT_NEAR(1, std::abs(params.normal.y), 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.z), 0.01);

	}

	TEST(Plane3, matching3Z) {

		std::vector<Point3f> pts = {
			Point3f(0,0,0),
			Point3f(1,0,0),
			Point3f(0,1,0)
		};

		const Plane3f::CoordinateParams params = PlaneEstimator3<float>::getParams(pts);
		ASSERT_NEAR(0, params.distance, 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.x), 0.01);
		ASSERT_NEAR(0, std::abs(params.normal.y), 0.01);
		ASSERT_NEAR(1, std::abs(params.normal.z), 0.01);

	}

}


#endif
