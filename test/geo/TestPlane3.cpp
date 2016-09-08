#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Plane3.h"
#include "../../cv/matching/PlaneDetection3.h"

#include <random>


#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotSplot.h"
#include "../../misc/gnuplot/GnuplotSplotElementLines.h"
#include "../../misc/gnuplot/GnuplotSplotElementPoints.h"

namespace K {

	void plot(Plane3f::CoordinateParams params, const std::vector<Point3f>& pts1, const std::vector<Point3f>& pts2) {

		static K::Gnuplot gp;

		K::GnuplotSplot plot;
		K::GnuplotSplotElementLines lines;		plot.add(&lines);
		K::GnuplotSplotElementPoints points1;	plot.add(&points1);	points1.setPointType(7); points1.setPointSize(0.75);
		K::GnuplotSplotElementPoints points2;	plot.add(&points2); points2.setPointType(7); points2.setPointSize(0.25); points2.setColorHex("#0000ff");

		for (const Point3f& p : pts1) {
			points1.add(K::GnuplotPoint3(p.x, p.y, p.z));
		}
		for (const Point3f& p : pts2) {
			points2.add(K::GnuplotPoint3(p.x, p.y, p.z));
		}


		gp << "set view equal xyz\n";
		gp << "set xrange[0:6]\n";
		gp << "set yrange[0:6]\n";
		gp << "set zrange[0:6]\n";

		const Point3f p1 = params.normal * params.distance;
		const Point3f p2 = p1 + params.normal;
		gp << "set arrow 1 from " << p1.x << "," << p1.y << "," << p1.z << " to " << p2.x << "," << p2.y << "," << p2.z << "\n";

		gp.draw(plot);
		gp.flush();

		usleep(1000*2000);


	}

}


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

	TEST(Plane3, distance) {

		{
			// very simple
			const Plane3f::CoordinateParams params(Point3f(0,0,1), 0);
			ASSERT_NEAR(0, params.getDistance(Point3f(0,0,0)), 0.01);
			ASSERT_NEAR(1, params.getDistance(Point3f(0,0,1)), 0.01);
			ASSERT_NEAR(2, params.getDistance(Point3f(0,0,2)), 0.01);
		}

		{
			// plane with distance
			const Plane3f::CoordinateParams params(Point3f(0,0,1), 1);
			ASSERT_NEAR(1, params.getDistance(Point3f(0,0,0)), 0.01);
			ASSERT_NEAR(0, params.getDistance(Point3f(0,0,1)), 0.01);
			ASSERT_NEAR(1, params.getDistance(Point3f(0,0,2)), 0.01);
		}

		{
			// plane with another distance
			const Plane3f::CoordinateParams params(Point3f(0,0,1), 2.5);
			ASSERT_NEAR(2.5, params.getDistance(Point3f(0,0,0)), 0.01);
			ASSERT_NEAR(1.5, params.getDistance(Point3f(0,0,1)), 0.01);
			ASSERT_NEAR(0.0, params.getDistance(Point3f(0,0,2.5)), 0.01);
			ASSERT_NEAR(0.5, params.getDistance(Point3f(0,0,3)), 0.01);

			// almost part of the plane (distance)
			ASSERT_NEAR(2.5, params.getDistance(Point3f(2,0,0)), 0.01);
			ASSERT_NEAR(2.5, params.getDistance(Point3f(0,2,0)), 0.01);

			// part of the plane
			ASSERT_NEAR(0.0, params.getDistance(Point3f(2,0,2.5)), 0.01);
			ASSERT_NEAR(0.0, params.getDistance(Point3f(0,2,2.5)), 0.01);
		}

		{
			// another plane
			const Plane3f::CoordinateParams params(Point3f(1,0,0), 1);
			ASSERT_NEAR(1.0, params.getDistance(Point3f(0,0,0)), 0.01);
			ASSERT_NEAR(0.0, params.getDistance(Point3f(1,0,0)), 0.01);
			ASSERT_NEAR(1.0, params.getDistance(Point3f(2,0,0)), 0.01);
			ASSERT_NEAR(2.0, params.getDistance(Point3f(3,0,0)), 0.01);
		}



	}


	TEST(Plane3, matching3_a) {

		Point3f v1(1,0,1);
		Point3f v2(0,1,0);
		Point3f vo(2,3,4);


		std::vector<Point3f> pts;
		std::minstd_rand gen;
		std::uniform_real_distribution<float> dist(0, 1);

		// estimation must work for 3 (or more!) points
		for (int i = 0; i < 50; ++i) {

			// add a new point
			Point3f p = v1 * dist(gen) + v2 * dist(gen) + vo;
			pts.push_back(p);

			// at least 3 points? -> check!
			if (pts.size() > 3) {
				// normal must be orthogonal to v1 and v2
				const Plane3f::CoordinateParams params = PlaneEstimator3<float>::getParams(pts);
				ASSERT_NEAR(0, params.normal.dot(v1), 0.01);
				ASSERT_NEAR(0, params.normal.dot(v2), 0.01);

//				if (pts.size() > 30) {
//					plot(params, pts);
//				}

			}


		}

	}


	TEST(Plane3, matchingRansac) {

		struct Desc {
			Point3f v1;
			Point3f v2;
			Point3f vo;
			Desc(const Point3f v1, const Point3f v2, const Point3f vo) : v1(v1), v2(v2), vo(vo) {;}
		};

		std::vector<Desc> descs = {
			Desc(Point3f(1,0,1), Point3f(0,1,0), Point3f(0,0,0)),
			Desc(Point3f(1,0,1), Point3f(0,1,0), Point3f(0,0,0)),
			Desc(Point3f(1,0,0), Point3f(0,1,0), Point3f(0,0,0)),
			Desc(Point3f(1,0,0), Point3f(0,1,0), Point3f(0,0,1)),
			Desc(Point3f(1,0,0), Point3f(0,1,0), Point3f(1,1,1)),
			Desc(Point3f(1,1,1), Point3f(1,0,0), Point3f(0,0,0)),
		};

		// try each setup
		for (const Desc& desc : descs) {

			std::vector<Point3f> pts;
			std::minstd_rand gen;

			std::uniform_real_distribution<float> dist(0, 4);
			std::uniform_real_distribution<float> distNoise(-0.15f, +0.15f);
			std::uniform_real_distribution<float> distOutlierValue(-0.5f, +2.5f);
			std::uniform_real_distribution<float> distOutlier(0, 1);


			for (int i = 0; i < 200; ++i) {

				// create a new point
				Point3f p = desc.v1 * dist(gen) + desc.v2 * dist(gen) + desc.vo;

				// generate 33% outliers that are strongly biased into a wrong direction
				if (distOutlier(gen) < 0.33) {
					Point3f pNoise(distOutlierValue(gen), distOutlierValue(gen), distOutlierValue(gen));
					p += pNoise;
				} else {
					Point3f pNoise(distNoise(gen), distNoise(gen), distNoise(gen));
					p += pNoise;
				}

				// add it
				pts.push_back(p);

			}

			// perform ransac
			PlaneEstimator3<float>::RANSAC ransac;
			ransac.setNumRuns(48);
			ransac.setMinMatchRate(0.5f);
			ransac.setMaxDistance(0.33f);

			const Plane3f::CoordinateParams params = ransac.get(pts);

			// debug-plot inliers and outliers
			std::vector<Point3f> pts1;
			std::vector<Point3f> pts2;
			for (const Point3f& p : pts) {
				if (params.getDistance(p) < 0.33) {pts1.push_back(p);} else {pts2.push_back(p);}
			}
			plot(params, pts1, pts2);

			// normal must be approx. orthogonal to v1 and v2
			ASSERT_NEAR(0, params.normal.dot(desc.v1), 0.05);
			ASSERT_NEAR(0, params.normal.dot(desc.v2), 0.05);

		}


	}

}




#endif
