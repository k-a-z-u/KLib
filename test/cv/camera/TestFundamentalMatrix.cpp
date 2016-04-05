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

		ImageChannel img(500,500); img.ones();
		Drawer d(img);

		for (int i = 0; i < 9; ++i) {

			const Eigen::Vector3d le = toVec(fm.getImgLeft(i));
			const Eigen::Vector3d ri = toVec(fm.getImgRight(i));
			const float res = (ri.transpose() * (fm.getFundamentalMatrix() * le));
			std::cout << "#" << res << std::endl;
//			ASSERT_GE(0.1, std::abs(res));

			const Eigen::Vector3d pl = toVec(fm.getImgLeft(i));
			const Eigen::Vector3d pr = toVec(fm.getImgRight(i));

			const Eigen::Vector3d lr = fm.getEpilineRight(pl);
			const Eigen::Vector3d ll = fm.getEpilineLeft(pr);

			for (int x = 0; x < 500; ++x) {
				int y1 = (-ll(0)*x - ll(2)) / ll(1);
				int y2 = (-lr(0)*x - lr(2)) / lr(1);
				if (x < 0) {continue;}
				if (x >= 500) {continue;}

				if (y1 >= 0 && y1 < 500) {img.set(x,y1,0);}
				if (y2 >= 0 && y2 < 500) {img.set(x,y2,0);}
			}

		}

		ImageFactory::writeJPEG("/tmp/1.jpg", img);

	}


}

#endif
