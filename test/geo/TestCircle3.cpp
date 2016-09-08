#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Circle3.h"
#include "../../cv/matching/CircleDetection3.h"

#include <random>
#include <fstream>




#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotSplot.h"
#include "../../misc/gnuplot/GnuplotSplotElementLines.h"
#include "../../misc/gnuplot/GnuplotSplotElementPoints.h"

namespace K {

	void plot(Circle3f params, const std::vector<Point3f>& pts1, const std::vector<Point3f>& pts2) {

		static K::Gnuplot gp;

		K::GnuplotSplot plot;
		K::GnuplotSplotElementLines lines;		plot.add(&lines);	lines.setColorHex("#888888");
		K::GnuplotSplotElementPoints points1;	plot.add(&points1);	points1.setPointType(7); points1.setPointSize(0.75);
		K::GnuplotSplotElementPoints points2;	plot.add(&points2); points2.setPointType(7); points2.setPointSize(0.25); points2.setColorHex("#0000ff");

		for (const Point3f& p : pts1) {
			points1.add(K::GnuplotPoint3(p.x, p.y, p.z));
		}
		for (const Point3f& p : pts2) {
			points2.add(K::GnuplotPoint3(p.x, p.y, p.z));
		}
		for (float r = 0; r < 2.0*(float)M_PI; r += 0.1f) {
			const Point3f p = params.getPointFor(r);
			lines.add(K::GnuplotPoint3(p.x, p.y, p.z));
		}

		const Point3f p1 = params.center;
		const Point3f p2 = params.center + params.normal;
		gp << "set view equal xyz\n";
		gp << "set xrange[-6:+6]\n";
		gp << "set yrange[-6:+6]\n";
		gp << "set zrange[-6:+6]\n";
		gp << "set arrow 1 from " << p1.x << "," << p1.y << "," << p1.z << " to " << p2.x << "," << p2.y << "," << p2.z << "\n";

		gp.draw(plot);
		gp.flush();

		usleep(1000*2000);


	}

}



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

	TEST(Circle3, ransac) {

		std::vector<Circle3f> circs = {

			Circle3f(Point3f(0,0,0), Point3f(0,0,1), 4),
			Circle3f(Point3f(1,0,0), Point3f(0,0,1), 4),
			Circle3f(Point3f(0,1,0), Point3f(0,0,1), 4),
			Circle3f(Point3f(0,0,1), Point3f(0,0,1), 4),
			Circle3f(Point3f(1,1,1), Point3f(0,0,1), 4),

			Circle3f(Point3f(0,0,0), Point3f(0,1,0), 4),
			Circle3f(Point3f(0,0,0), Point3f(0.4,0.3,1), 4),
			Circle3f(Point3f(1,0.5,0.75), Point3f(0.3,0.2,0.7), 4)

		};

		// try each of the configured circles above
		for (Circle3f circ : circs) {

			std::minstd_rand gen;
			std::uniform_real_distribution<float> distRad(0, M_PI*2);
			std::uniform_real_distribution<float> distNoise(-0.25, +0.25);
			std::uniform_real_distribution<float> distOutlier(-1.0, +2.5);
			std::uniform_real_distribution<float> distIsOutlier(0, 1);

			std::vector<Point3f> pts;

			for (int i = 0; i < 250; ++i) {

				const float rad = distRad(gen);
				Point3f pos = circ.getPointFor(rad);

				// 33% outliers
				if (distIsOutlier(gen) < 0.33) {
					const Point3f noise(distOutlier(gen), distOutlier(gen), distOutlier(gen));
					pos += noise;
				} else {
					const Point3f noise(distNoise(gen), distNoise(gen), distNoise(gen));
					pos += noise;
				}

				pts.push_back(pos);

			}

			CircleEstimator3<float>::RANSAC ransac;
			ransac.setMaxDistance(0.30);
			ransac.setNumRuns(48);
			ransac.setMinMatchRate(0.50);

			const Circle3f est = ransac.get(pts);


			// debug plot
			std::vector<Point3f> pts1;
			std::vector<Point3f> pts2;
			for (const Point3f p : pts) {
				if (est.getDistanceFromOutline(p) > ransac.getMaxDistance()) {pts2.push_back(p);} else {pts1.push_back(p);}
			}
			plot(est, pts1, pts2);


			ASSERT_NEAR(circ.center.x, est.center.x, 0.15);
			ASSERT_NEAR(circ.center.y, est.center.y, 0.15);
			ASSERT_NEAR(circ.center.z, est.center.z, 0.15);

			ASSERT_NEAR(circ.normal.x, est.normal.x, 0.10);
			ASSERT_NEAR(circ.normal.y, est.normal.y, 0.10);
			ASSERT_NEAR(circ.normal.z, est.normal.z, 0.10);

			ASSERT_NEAR(circ.radius, est.radius, 0.15);

		}

	}


}


#endif
