#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Circle3.h"
#include "../../cv/matching/CircleDetection3.h"

#include <random>
#include <fstream>

namespace K {

	// circle with normal = z-axis
	TEST(Circle3, getZ) {
		const Circle3f circ(Point3f(0,0,0), Point3f(0,0,1), 5);
		ASSERT_NEAR(+5.0f, circ.getPointFor((float)M_PI*0.0f).x, 0.01f);
		ASSERT_NEAR(+5.0f, circ.getPointFor((float)M_PI*0.5f).y, 0.01f);
		ASSERT_NEAR(-5.0f, circ.getPointFor((float)M_PI*1.0f).x, 0.01f);
		ASSERT_NEAR(-5.0f, circ.getPointFor((float)M_PI*1.5f).y, 0.01f);
	}

	// circle with normal = y-axis [z,y,+,-] might be shifted due to a degree of freedom while rotating from (0,1,0) to (0,0,1)
	TEST(Circle3, getY) {
		const Circle3f circ(Point3f(0,0,0), Point3f(0,1,0), 4);
		ASSERT_NEAR(+4.0f, std::abs(circ.getPointFor((float)M_PI*0.0f).x), 0.01f);
		ASSERT_NEAR(+4.0f, std::abs(circ.getPointFor((float)M_PI*0.5f).z), 0.01f);
		ASSERT_NEAR(+4.0f, std::abs(circ.getPointFor((float)M_PI*1.0f).x), 0.01f);
		ASSERT_NEAR(+4.0f, std::abs(circ.getPointFor((float)M_PI*1.5f).z), 0.01f);
	}

	// circle with normal = x-axis [z,y,+,-] might be shifted due to a degree of freedom while rotating from (1,0,0) to (0,0,1)
	TEST(Circle3, getX) {
		const Circle3f circ(Point3f(0,0,0), Point3f(1,0,0), 3);
		ASSERT_NEAR(+3.0f, std::abs(circ.getPointFor((float)M_PI*0.0f).z), 0.01f);
		ASSERT_NEAR(+3.0f, std::abs(circ.getPointFor((float)M_PI*0.5f).y), 0.01f);
		ASSERT_NEAR(+3.0f, std::abs(circ.getPointFor((float)M_PI*1.0f).z), 0.01f);
		ASSERT_NEAR(+3.0f, std::abs(circ.getPointFor((float)M_PI*1.5f).y), 0.01f);
	}

	TEST(Circle3, estimate) {

		//std::cout << "TODO: re-enable circle3::estimate" << std::endl;
		//return;

		struct Desc {
			Point3f center;
			Point3f normal;
			float radius;
			Desc(const Point3f center, const Point3f normal, const float radius) : center(center), normal(normal), radius(radius) {;}
		};

		std::vector<Desc> combinations = {

			Desc(Point3f(0,0,0), Point3f(0,0,1), 3),
			Desc(Point3f(0,0,1), Point3f(0,0,1), 2),
			Desc(Point3f(0,1,0), Point3f(0,0,1), 2),
			Desc(Point3f(1,0,0), Point3f(0,0,1), 2),
			Desc(Point3f(1,2,3), Point3f(0,0,1), 2),

			Desc(Point3f(0,0,0), Point3f(0,1,0), 4),
			Desc(Point3f(3,2,1), Point3f(0,1,0), 5),

			Desc(Point3f(0,0,0), Point3f(1,0,0), 7),
			Desc(Point3f(2,1,3), Point3f(1,0,0), 3),

			Desc(Point3f(0,0,0), Point3f(1,1,0), 4),		// unnormalized normal
			Desc(Point3f(2,1,3), Point3f(1,1,0), 2),		// unnormalized normal

		};

		for (const Desc& desc : combinations) {

			// the circle to detect
			const Circle3f circ(desc.center, desc.normal, desc.radius);

			// get 3 points along this circle
			std::vector<Point3f> pts = {
				circ.getPointFor(0.0f),
				circ.getPointFor(1.0f),
				circ.getPointFor(2.0f),
			};

			// try detection
			Circle3<float> gotCirc = CircleEstimator3<float>::getParams(pts);

			// compare detected values with original ones
			ASSERT_NEAR(0, circ.center.getDistance(gotCirc.center), 0.1);
			ASSERT_NEAR(0, circ.normal.getAbs().getDistance(gotCirc.normal.getAbs()), 0.1);
			ASSERT_NEAR(circ.radius, gotCirc.radius, 0.1);

		}


	}


	TEST(Circle3, data) {

		std::ifstream inp("/tmp/rings2.dat");

		std::vector<std::vector<Point3f>> circles;
		circles.resize(1);


		while(inp.is_open() && !inp.bad() && !inp.eof()) {

			char line[4096];
			inp.getline(line, 4096);

			std::stringstream ss(line);
			if (ss.str().empty()) {circles.resize(circles.size()+1); continue;}


			K::Point3f p;
			ss >> p.x;
			ss >> p.y;
			ss >> p.z;

			circles.back().push_back(p);

		}

		std::ofstream out("/tmp/circles3.dat");

		for (std::vector<Point3f>& circlePoints : circles) {

			if (circlePoints.size() < 3) {continue;}

			Point3f sum;
			for (Point3f p : circlePoints) {
				sum += p;
			}
			Point3f avg = sum / circlePoints.size();


			Circle3<float> est = CircleEstimator3<float>::getParams(circlePoints);
			std::cout << est.center.asString() << std::endl;
			std::cout << avg.asString() << std::endl;
			std::cout << std::endl;

			for (float r = 0; r < 2*M_PI; r += 0.1) {
				Point3f pt = est.getPointFor(r);
				out << pt.x << " " << pt.y << " " << pt.z << "\n";
			}

		}

		out.close();

	}



}


#endif
