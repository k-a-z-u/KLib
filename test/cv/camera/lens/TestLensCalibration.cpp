#ifndef TESTLENSCALIBRATION_CPP
#define TESTLENSCALIBRATION_CPP

#ifdef WITH_TESTS

#include "../../../Test.h"
#include "../../../../cv/camera/lens/LensDistortionCalibration.h"
#include "../../../../cv/ImageFactory.h"
#include "../../../../cv/features/CornerDetectorHarris.h"

namespace K {

	TEST(LensCalibration, calibrate) {

		std::vector<LensDistortionCalibration::Path> paths;

//		LensDistortionCalibration::Path p1;
//		p1.add(Point2f(152,95));
//		p1.add(Point2f(172,96));
//		p1.add(Point2f(192,97));
//		p1.add(Point2f(213,99));
//		p1.add(Point2f(234,101));
//		p1.add(Point2f(254,104));
//		p1.add(Point2f(275,106));
//		p1.add(Point2f(295,108));
//		paths.push_back(p1);

//		LensDistortionCalibration::Path p2;
//		p2.add(Point2f(142,198));
//		p2.add(Point2f(162,199));
//		p2.add(Point2f(182,201));
//		p2.add(Point2f(203,203));
//		p2.add(Point2f(224,205));
//		p2.add(Point2f(245,208));
//		p2.add(Point2f(265,209));
//		p2.add(Point2f(286,212));
//		paths.push_back(p2);

		LensDistortionCalibration::Path p1;
		LensDistortionCalibration::Path p2;
		LensDistortionCalibration::Path p3;
		LensDistortionCalibration::Path p4;

		p1.add(Point2f(76,77));
		p1.add(Point2f(194,71));
		p1.add(Point2f(377,73));
		p1.add(Point2f(436,77));

		p2.add(Point2f(71,194));
		p2.add(Point2f(192,192));
		p2.add(Point2f(319,192));
		p2.add(Point2f(440,195));

		p3.add(Point2f(76,436));
		p3.add(Point2f(134,439));
		p3.add(Point2f(194,441));
		p3.add(Point2f(255,441));
		p3.add(Point2f(317,441));
		p3.add(Point2f(377,439));
		p3.add(Point2f(436,436));

		p4.add(Point2f(431,490));
		p4.add(Point2f(436,436));
		p4.add(Point2f(436,436));
		p4.add(Point2f(439,378));
		p4.add(Point2f(440,317));
		p4.add(Point2f(441,256));
		p4.add(Point2f(440,194));
		p4.add(Point2f(439,134));
		p4.add(Point2f(436,76));
		p4.add(Point2f(489,79));

		paths.push_back(p1);
		paths.push_back(p2);
		paths.push_back(p3);
		paths.push_back(p4);

		std::vector<float> params = LensDistortionCalibration::calibrate(paths, Point2f(512, 512), 2);

		//params[0] = +0.5;
		//params[1] = -0.5;

		ImageChannel src = ImageFactory::readPNG(getDataFile("lens_1.png"));
		ImageChannel dst = LensDistortionRadial::undistort(src, params.data(), params.size());
		ImageFactory::writePNG("/tmp/test_1.png", dst);

	}

	TEST(LensCalibration, calibrate2) {



		std::vector<LensDistortionCalibration::Path> paths;

		LensDistortionCalibration::Path p1;
		LensDistortionCalibration::Path p2;
		LensDistortionCalibration::Path p3;

		p1.add(Point2f(80,83)).add(Point2f(123,79)).add(Point2f(165,77));
		p2.add(Point2f(352,418)).add(Point2f(409,416)).add(Point2f(438,413));
		p3.add(Point2f(71,446)).add(Point2f(68,407)).add(Point2f(65,364));

		paths.push_back(p1);
		paths.push_back(p2);
		paths.push_back(p3);

		std::vector<float> params = LensDistortionCalibration::calibrate(paths, Point2f(512, 512), 2);

		//params[0] = +0.5;
		//params[1] = -0.5;


//		see youtube videos laptop d:\kiste

		ImageChannel src = ImageFactory::readPNG(getDataFile("lens_2.png"));
		ImageChannel dst = LensDistortionRadial::undistort(src, params.data(), params.size());
		ImageFactory::writePNG("/tmp/test_2.png", dst);

		CornerDetectorHarris cdh;
		std::vector<Corner> corners = cdh.getCorners(src);

		ImageChannel i2(src.getWidth(), src.getHeight());
		for (Corner c : corners) {
			i2.set(c.x, c.y, 1);
		}
		ImageFactory::writePNG("/tmp/test_3.png", i2);


	}

}

#endif


#endif // TESTLENSCALIBRATION_CPP
