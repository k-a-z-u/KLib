#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/FundamentalMatrix.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"

namespace K {

	Eigen::Vector3d toVec(const Point2f p) {return Eigen::Vector3d(p.x, p.y, 1);}

	TEST(FundamentalMatrix, estimate) {

		FundamentalMatrix fm;
		fm.addCorrespondence(85,53,		29,57);
		fm.addCorrespondence(264,34,	209,36);
		fm.addCorrespondence(362,32,	306,32);

		fm.addCorrespondence(213,155,	155,159);

		fm.addCorrespondence(96,209,	36,210);
		fm.addCorrespondence(330,212,	269,211);
		fm.addCorrespondence(276,241,	216,242);

		fm.addCorrespondence(385,332,	321,328);
		fm.addCorrespondence(180,389,	114,388);
		fm.addCorrespondence(136,500,	67,500);

		fm.estimate();


		ImageChannel img(600,600);
		img.ones();
		Drawer d(img);


		for (int i = 0; i < 9; ++i) {
			const Eigen::Vector3d a = toVec(fm.getImg1(i));
//			const Eigen::Vector3d b = toVec(fm.getImg2(i));
//			const float res = (a.transpose() * (fm.get() * b));
//			std::cout << "#" << res << std::endl;
//			ASSERT_GE(0.1, std::abs(res));
			Eigen::Vector3d l = fm.get() * a;
			std::cout << "#" << l << std::endl;
			l /= l(2);
			//l *= 50;

			Eigen::Vector3d s = a*20;
			d.drawLine(l(0), l(1), l(0)*100, l(1)*100);

		}

		ImageFactory::writeJPEG("/tmp/1.jpg", img);

	}


}

#endif
