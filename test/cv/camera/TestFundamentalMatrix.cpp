#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/FundamentalMatrix.h"
#include "../../../cv/ImageFactory.h"

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

		for (int i = 0; i < 9; ++i) {
			const Eigen::Vector3d a = toVec(fm.getImg1(i));
			const Eigen::Vector3d b = toVec(fm.getImg2(i));
			const float res = (a.transpose() * (fm.get() * b));
			std::cout << "#" << res << std::endl;
			ASSERT_GE(0.1, std::abs(res));
		}

	}


}

#endif
