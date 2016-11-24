#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/ImageChannel.h"
#include "../../../cv/matching/EllipseEstimatorMath.h"

using namespace K;

std::vector<K::Point2f> getTestEllipse(const K::Ellipse::GeometricParams& geo, const float segSize, const int numSamples) {



	std::vector<K::Point2f> pts;

	const float start = -segSize;
	const float end = +segSize;

	for (int i = 0; i < numSamples; ++i) {
		const float rad = start + (end-start) * i / (numSamples-1);
		K::Point2f pt = geo.getPointFor(rad);
		pts.push_back(pt);
	}

	return pts;

//	std::vector<K::Point2i> pts = {

//		K::Point2i(-4,0),
//		K::Point2i(-3,2),
//		K::Point2i(-2,3),

//		K::Point2i(0,4),

//		K::Point2i(2,3),
//		K::Point2i(3,2),
//		K::Point2i(4,0),

//	};

//	return pts;

}

void dumpEllipse(K::Ellipse::CanonicalParams canon) {

	canon.fixF();
	K::Ellipse::GeometricParams geo = canon.toGeometric();
	std::cout << " - (" << geo.center.x << "," << geo.center.y << ") (" << geo.a << "," << geo.b << ")" << std::endl;

}

void dumpEllipse(K::Ellipse::GeometricParams geo) {
	dumpEllipse(geo.toCanonical());
}

TEST(EllipseEV, ev) {

//	std::vector<K::Point2i> pts = {
//		K::Point2i(1,6),
//		K::Point2i(2,7),
//		K::Point2i(3,8),
//		K::Point2i(4,9),
//		K::Point2i(5,0),
//		K::Point2i(6,1),
//	};

//	EllipseEstimator::Estimation::getParamsEV(pts);

}

void testDump(const K::Ellipse::GeometricParams& ellipse, const std::vector<K::Point2f>& pts) {

	std::cout << "SVD" << std::endl;
	EllipseEstimator::Estimation e1 = EllipseEstimator::Estimation::getParams<float>( pts );
	dumpEllipse(e1.toEllipse());

	std::cout << "normalized EV" << std::endl;
	EllipseEstimator::Estimation e2 = EllipseEstimator::Estimation::getParamsNormalized<float>( pts );
	dumpEllipse(e2.toEllipse());

	std::cout << "renormalization" << std::endl;
	EllipseEstimator::Estimation e3 = EllipseEstimator::Estimation::getParamsRenormalized<float>( pts );
	dumpEllipse(e3.toEllipse());

	std::cout << std::endl;

}

TEST(EllipseEV, modes) {

	// fairly large ellipse segment, 12 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(0,0), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/4.0f, 12);
		testDump(geo, pts);
	}

	// fairly large ellipse segment, 12 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/4.0f, 12);
		testDump(geo, pts);
	}

	// fairly large ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/4.0f, 6);
		testDump(geo, pts);
	}

	// shorter ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/6.0f, 6);
		testDump(geo, pts);
	}

	// even shorter ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/16.0f, 6);
		testDump(geo, pts);
	}

	// even shorter ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/32.0f, 6);
		testDump(geo, pts);
	}

	// even shorter ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/64.0f, 6);
		testDump(geo, pts);
	}

	// even shorter ellipse segment, 6 samples
	{
		K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
		const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/96.0f, 6);
		testDump(geo, pts);
	}

}

TEST(EllipseEV, benchmark) {

	const K::Ellipse::GeometricParams geo(K::Point2f(2,4), 10, 5, 0);
	const std::vector<K::Point2f> pts = getTestEllipse(geo, (float)M_PI/96.0f, 6);

	for (int i = 0; i < 40960; ++i) {
		//EllipseEstimator::Estimation e1 = EllipseEstimator::Estimation::getParams<float>(pts);				// 5650ms [Dbg], 225 Release
		EllipseEstimator::Estimation e1 = EllipseEstimator::Estimation::getParamsNormalized<float>(pts);		// 15833ms [Dbg], 320ms release
	}

}

/*
TEST(EllipseEV, svd) {

	EllipseEstimator::Estimation est = EllipseEstimator::Estimation::getParams<float>( getTestEllipse1() );

	K::Ellipse::CanonicalParams canon = est.toEllipse();
	dumpEllipse(canon);

}

TEST(EllipseEV, evNormAlt) {

	EllipseEstimator::Estimation est = EllipseEstimator::Estimation::getParamsEVNormalized( getTestEllipse1() );
	std::cout << "OK" << std::endl;

	K::Ellipse::CanonicalParams canon = est.toEllipse();
	dumpEllipse(canon);

}

TEST(EllipseEV, evNorm) {

	EllipseEstimator::Estimation est = EllipseEstimator::Estimation::getParamsRenormalized( getTestEllipse1() );
	std::cout << "OK" << std::endl;

	K::Ellipse::CanonicalParams canon = est.toEllipse();
	dumpEllipse(canon);

}
*/


#endif


