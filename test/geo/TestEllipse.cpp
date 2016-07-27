
#ifdef WITH_TESTS

#include "../Test.h"
#include "../../geo/Ellipse.h"
#include "../../cv/matching/EllipseDetection.h"

#include "../../cv/ImageFactory.h"
#include "../../cv/segmentation/RegionGrowing.h"

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

	TEST(Ellipse, estimateFromImage) {

		// ellipse image
		const ImageChannel img = ImageFactory::readPNG( getDataFile("ellipse1.png") );

		// one seed per ellipse
		const std::vector<Point2i> seeds = {
			K::Point2i(52,8),		// topmost
			K::Point2i(36,20),		// topmost inner
			K::Point2i(58,58),		// middle
			K::Point2i(31,83),		// lower left
			K::Point2i(61,105),		// lower right

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

			// estimate and check error
			float errorScore;
			K::Ellipse::CanonicalParams cano = EllipseEstimator::get(segment.points, errorScore);
			ASSERT_LE(errorScore, 0.4);

			// compare with expected values
			Ellipse::GeometricParams geo = cano.toGeometric();
			ASSERT_NEAR(geo.center.x,	expected[i].center.x,	1.0);
			ASSERT_NEAR(geo.center.y,	expected[i].center.y,	1.0);
			ASSERT_NEAR(geo.a,			expected[i].a,			1.0);
			ASSERT_NEAR(geo.b,			expected[i].b,			1.0);
			ASSERT_NEAR(geo.rad,		expected[i].rad,		0.1);

		}


	}




}

#endif
