
#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Ellipse.h"
#include "../../cv/matching/EllipseDetection.h"

#include "../../cv/draw/Drawer.h"
#include "../../cv/ImageFactory.h"
#include "../../cv/segmentation/RegionGrowing.h"
#include "../../cv/segmentation/Segmentation.h"

namespace K {

	TEST(Ellipse, paramConvert) {

		Ellipse::GeometricParams geo (K::Point2f(10, 15), 8, 6, 1.2f);		// geo
		Ellipse::CanonicalParams can = geo.toCanonical();					// geo -> can
		Ellipse::GeometricParams geo2 = can.toGeometric();					// can -> geo

		ASSERT_NEAR(geo.center.x, geo2.center.x, 0.001);
		ASSERT_NEAR(geo.center.y, geo2.center.y, 0.001);
		ASSERT_NEAR(geo.a, geo2.a, 0.001);
		ASSERT_NEAR(geo.b, geo2.b, 0.001);
		ASSERT_NEAR(geo.rad, geo2.rad, 0.001);

	}


	TEST(Ellipse, paramConvert2) {

		Ellipse::GeometricParams geo (K::Point2f(10, 15), 8, 6, 1.2f);
		Ellipse::VectorParams vec = geo.toVector();

		// geo and vec must provide the same ellipse points for a given angle
		ASSERT_NEAR(0, geo.getPointFor(0).getDistance(vec.getPointFor(0)), 0.01);
		ASSERT_NEAR(0, geo.getPointFor(1).getDistance(vec.getPointFor(1)), 0.01);
		ASSERT_NEAR(0, geo.getPointFor(2).getDistance(vec.getPointFor(2)), 0.01);
		ASSERT_NEAR(0, geo.getPointFor(3).getDistance(vec.getPointFor(3)), 0.01);
		ASSERT_NEAR(0, geo.getPointFor(4).getDistance(vec.getPointFor(4)), 0.01);
		ASSERT_NEAR(0, geo.getPointFor(5).getDistance(vec.getPointFor(5)), 0.01);

	}

	TEST(Ellipse, getPointFor) {

		Ellipse::GeometricParams geo (K::Point2f(0, 0), 8, 3, 0.0f);

		ASSERT_NEAR(8, geo.getPointFor(0).x, 0.001);		// 0° = major-axis to the right
		ASSERT_NEAR(0, geo.getPointFor(0).y, 0.001);

		ASSERT_NEAR(0, geo.getPointFor((float)M_PI_2).x, 0.001);	// 90° = minor-axis to the top
		ASSERT_NEAR(3, geo.getPointFor((float)M_PI_2).y, 0.001);

	}

	TEST(Ellipse, getNearest) {

		Ellipse::GeometricParams geo (K::Point2f(0, 0), 10, 2, 0.0f);
		Ellipse::DistanceEstimatorBisect bisect(geo);
		bisect.getNearest(K::Point2f(15, 15));

		//Ellipse::GeometricParams geo (K::Point2f(0, 0), 10, 2, 0.9f);
		//EllipseEstimator::getNearest(geo, K::Point2f(20, 0));

	}

	TEST(Ellipse, fromPhoto) {

		// ellipse image
		const ImageChannel img = ImageFactory::readPNG( getDataFile("ellipseImg2.png") );

		// get all segments
		std::vector<Segment> segments = Segmentation::getSegments(img);

		K::ImageChannel iOut(img.getWidth(), img.getHeight()); iOut.ones();
		K::Drawer drawer(iOut);

		// process each segment
		for (const Segment& seg : segments) {

			// skip small segments
			if (seg.points.size() < 100) {continue;}
			// skip large segments
			//if (seg.points.size() > 3500) {continue;}
			//  skip black segments (only use white borders)
			if (seg.avg == 0) {continue;}

			// process
			//K::Ellipse::CanonicalParams cano = EllipseEstimator::getRemoveWorst(seg.points);
			K::Ellipse::CanonicalParams cano = EllipseEstimator::getRANSAC(seg.points);

			// convert
			K::Ellipse::GeometricParams geo = cano.toGeometric();

			// skip large
			if (geo.a > 125 || geo.b > 125) {continue;}

			// debug draw
			drawer.drawEllipse(geo);

		}

		K::ImageFactory::writePNG("/tmp/2.png", iOut);

	}

	TEST(Ellipse, estimateFromImage) {

		// ellipse image
		const ImageChannel img = ImageFactory::readPNG( getDataFile("ellipse1.png") );

		// one seed per ellipse
		const std::vector<Point2i> seeds = {
			K::Point2i(52,8),		// topmost
			K::Point2i(36,20),		// topmost inner
			K::Point2i(58,58),		// middle
			K::Point2i(31,83),		// lower left
			K::Point2i(61,105),		// lower rightget

		};

		// expected values for center, a/b and radius
		const std::vector<Ellipse::GeometricParams> expected = {
			Ellipse::GeometricParams(K::Point2f(64,29), 59, 21.5, 0),
			Ellipse::GeometricParams(K::Point2f(67,30), 31, 3.5f, 0.3f),
			Ellipse::GeometricParams(K::Point2f(64,66), 57, 7, 0),
			Ellipse::GeometricParams(K::Point2f(31,101), 18.5f, 7.5f, (float)-M_PI/2),
			Ellipse::GeometricParams(K::Point2f(89,105), 28, 14, 0),
		};

		// estimate each ellipse
		for (int i = 0; i < (int)seeds.size(); ++i) {

			// get the segment connected to the seed
			Segment segment = RegionGrowing::get(img, seeds[i]);

			// estimate
			K::Ellipse::CanonicalParams cano = EllipseEstimator::get(segment.points);

			// compare with expected values
			Ellipse::GeometricParams geo = cano.toGeometric();
			ASSERT_NEAR(geo.center.x,	expected[i].center.x,	1.0);
			ASSERT_NEAR(geo.center.y,	expected[i].center.y,	1.0);
			ASSERT_NEAR(geo.a,			expected[i].a,			1.0);
			ASSERT_NEAR(geo.b,			expected[i].b,			1.0);
			ASSERT_NEAR(geo.rad,		expected[i].rad,		0.1);

		}

	}


	TEST(Ellipse, estimateFromImageRANSAC) {




		// ellipse image
		const ImageChannel img = ImageFactory::readPNG( getDataFile("ellipse2.png") );

		// one seed per ellipse
		const std::vector<Point2i> seeds = {
			K::Point2i(52,8),		// topmost
			K::Point2i(57,23),		// topmost inner
			K::Point2i(58,58),		// middle
			K::Point2i(34,84),		// lower left
			K::Point2i(61,105),		// lower right
		};

		// expected values for center, a/b and radius
		const std::vector<Ellipse::GeometricParams> expected = {
			Ellipse::GeometricParams(K::Point2f(64,29), 59, 21.5, 0),
			Ellipse::GeometricParams(K::Point2f(69,30), 31, 3.5f, 0.3f),
			Ellipse::GeometricParams(K::Point2f(64,66), 57, 7, 0),
			Ellipse::GeometricParams(K::Point2f(31,101), 18.5f, 7.5f, (float)-M_PI/2),
			Ellipse::GeometricParams(K::Point2f(89,105), 28, 14, 0),
		};

		K::ImageChannel iOut(128,128); iOut.ones();
		K::Drawer drawer(iOut);

		// estimate each ellipse
		for (int i = 0; i < (int)seeds.size(); ++i) {

			// get the segment connected to the seed
			const Segment segment = RegionGrowing::get(img, seeds[i]);

			// estimate and check error
			K::Ellipse::CanonicalParams cano = EllipseEstimator::getRANSAC(segment.points);
			//K::Ellipse::CanonicalParams cano = EllipseEstimator::getRemoveWorst(segment.points);

			drawer.drawEllipse(cano.toGeometric());

//			// compare with expected values
//			Ellipse::GeometricParams geo = cano.toGeometric();
//			ASSERT_NEAR(geo.center.x,	expected[i].center.x,	1.0);
//			ASSERT_NEAR(geo.center.y,	expected[i].center.y,	1.0);
//			ASSERT_NEAR(geo.a,			expected[i].a,			3.0);
//			ASSERT_NEAR(geo.b,			expected[i].b,			1.0);
//			ASSERT_NEAR(geo.rad,		expected[i].rad,		0.1);

		}

		K::ImageFactory::writePNG("/tmp/1.png", iOut);


	}




}

#endif
