#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/FundamentalMatrix.h"
#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"

#include "../../../misc/gnuplot/Gnuplot.h"
#include "../../../misc/gnuplot/GnuplotPlot.h"
#include "../../../misc/gnuplot/GnuplotPlotElementPoints.h"
#include "../../../misc/gnuplot/GnuplotPlotElementLines.h"
#include "../../../misc/gnuplot/GnuplotPlotElementImage.h"
#include "../../../misc/gnuplot/GnuplotMultiplot.h"

#include "../../../cv/matching/Matching.h"
#include "../../../cv/matching/MatchingSAD.h"

#include "../../../cv/draw/BresenhamIter.h"

#include "../../../cv/filter/Normalize.h"


namespace K {

	Eigen::Vector3d toVec(const Point2f p) {return Eigen::Vector3d(p.x, p.y, 1);}


	class StereoReconstruction {

	private:

		ImageChannel imgLeft;
		ImageChannel imgRight;
		FundamentalMatrix fm;

	public:

		/** ctor */
		StereoReconstruction(ImageChannel& imgLeft, ImageChannel& imgRight, FundamentalMatrix& fm) :
			imgLeft(imgLeft), imgRight(imgRight), fm(fm) {
			;
		}

		void perform() {

			MatchingSAD sad(imgLeft, imgRight, 9);
			ImageChannel imgDepth(imgLeft.getWidth(), imgLeft.getHeight());
			imgDepth.ones();


			#pragma omp parallel for
			for (int y = 0; y < imgLeft.getHeight(); ++y) {
				for (int x = 0; x < imgLeft.getWidth(); ++x) {

					const Point2i pL(x,y);

					const Eigen::Vector3d l = fm.getEpilineRight(pL);
					const int x1 = 0;
					const int x2 = imgLeft.getWidth();
					const int y1 = -(l(0)*x1 + l(2)) / l(1);
					const int y2 = -(l(0)*x2 + l(2)) / l(1);

					BresenhamIter iter(x1,y1,x2,y2);

					Point2i pMin(0,0);
					float vMin = INFINITY;

					while (iter.hasNext()) {
						const Point2i pR = iter.next();
						if (pR.x < 0 || pR.y < 0 || pR.x > imgRight.getWidth() || pR.y > imgRight.getHeight()) {continue;}
						const float err = sad.getError(pL, pR);
						if (err < vMin) {vMin = err; pMin = pR;}
					}

					if (vMin != INFINITY) {
						imgDepth.set(pL.x, pL.y, pL.getDistance(pMin));
					}

				}
			}

			Normalize::inplace(imgDepth);
			ImageFactory::writePNG("/tmp/depth.png", imgDepth);


		}

	};


	TEST(FundamentalMatrix, estimate) {

		const std::string fileLeft = getDataFile("stereo1.jpg");		// left image
		const std::string fileRight = getDataFile("stereo2.jpg");		// right image

		ImageChannel imgLeft = ImageFactory::readJPEG(fileLeft);
		ImageChannel imgRight = ImageFactory::readJPEG(fileRight);




		FundamentalMatrix fm;
		MatchingSAD sad(imgLeft, imgRight);
		Matching matcher;

		{

			// image1				// image2 (left of image1)	// refine the approximate matching positions
			Point2i pl1(85,53);		Point2i pr1(29,57);			pr1 = matcher.refine(sad, pl1, pr1);
			Point2i pl2(264,34);	Point2i pr2(209,36);		pr2 = matcher.refine(sad, pl2, pr2);
			Point2i pl3(362,32);	Point2i pr3(306,32);		pr3 = matcher.refine(sad, pl3, pr3);

			Point2i pl4(213,155);	Point2i pr4(155,159);		pr4 = matcher.refine(sad, pl4, pr4);

			Point2i pl5(96,209);	Point2i pr5(36,210);		pr5 = matcher.refine(sad, pl5, pr5);
			Point2i pl6(330,212);	Point2i pr6(269,211);		pr6 = matcher.refine(sad, pl6, pr6);
			Point2i pl7(276,241);	Point2i pr7(216,242);		pr7 = matcher.refine(sad, pl7, pr7);

			Point2i pl8(385,332);	Point2i pr8(321,328);		pr8 = matcher.refine(sad, pl8, pr8);
			Point2i pl9(180,389);	Point2i pr9(114,388);		pr9 = matcher.refine(sad, pl9, pr9);
			Point2i pl10(136,500);	Point2i pr10(67,500);		pr10 = matcher.refine(sad, pl10, pr10);

			fm.addCorrespondence(pl1,	pr1);
			fm.addCorrespondence(pl2,	pr2);
			fm.addCorrespondence(pl3,	pr3);

			fm.addCorrespondence(pl4,	pr4);

			fm.addCorrespondence(pl5,	pr5);
			fm.addCorrespondence(pl6,	pr6);
			fm.addCorrespondence(pl7,	pr7);
			fm.addCorrespondence(pl8,	pr8);
			//fm.addCorrespondence(pl9,	pr9);
			//fm.addCorrespondence(pl10,	pr10);

		}

		fm.estimate();

		StereoReconstruction sr(imgLeft, imgRight, fm);
		sr.perform();

		ImageChannel img(500,500); img.ones();
		Drawer d(img);



		Gnuplot gp;
		GnuplotMultiplot multiplot(1,2);

		GnuplotPlot plotL;
		GnuplotPlotElementPoints ptL; ptL.setColorHex("#FFFF00"); ptL.setPointSize(0.8); ptL.setPointType(7);
		GnuplotPlotElementLines plL; plL.setColorHex("#0000ff");
		GnuplotPlotElementImage pimgL(fileLeft);	// left image
		plotL.add(&pimgL);
		plotL.add(&plL);
		plotL.add(&ptL);

		GnuplotPlot plotR;
		GnuplotPlotElementPoints ptR; ptR.setColorHex("#FFFF00"); ptR.setPointSize(0.8); ptR.setPointType(7);
		GnuplotPlotElementLines plR; plR.setColorHex("#0000ff");
		GnuplotPlotElementImage pimgR(fileRight); // right image
		plotR.add(&pimgR);
		plotR.add(&plR);
		plotR.add(&ptR);

		multiplot.add(&plotL);
		multiplot.add(&plotR);





		for (int i = 0; i < 8; ++i) {

			const Eigen::Vector3d pil = toVec(fm.getPointLeft(i));
			const Eigen::Vector3d pir = toVec(fm.getPointRight(i));
			const float res = (pir.transpose() * (fm.getFundamentalMatrix() * pil));
			std::cout << "#" << res << std::endl;
			//ASSERT_GE(0.1, std::abs(res));
		}

		for (int i = 0; i < 9; ++i) {
	//	const int i = 1;

			const Eigen::Vector3d piL = toVec(fm.getPointLeft(i));
			const Eigen::Vector3d piR = toVec(fm.getPointRight(i));

			const Eigen::Vector3d lR = fm.getEpilineRight(piL);
			const Eigen::Vector3d lL = fm.getEpilineLeft(piR);

			ptL.add(GnuplotPoint2(piL(0), piL(1)));
			ptR.add(GnuplotPoint2(piR(0), piR(1)));

				for (int xx = -300; xx < 600; xx+=25) {
					int x1 = xx;
					int x2 = xx + 25;

				{
					float y1 = -(lL(0)*x1 + lL(2)) / lL(1);
					float y2 = -(lL(0)*x2 + lL(2)) / lL(1);
					plL.addSegment(GnuplotPoint2(x1,y1), GnuplotPoint2(x2,y2));
				}

				{
					float y1 = -(lR(0)*x1 + lR(2)) / lR(1);
					float y2 = -(lR(0)*x2 + lR(2)) / lR(1);
					plR.addSegment(GnuplotPoint2(x1,y1), GnuplotPoint2(x2,y2));
				}

			}




			//}

		}

		ImageFactory::writeJPEG("/tmp/1.jpg", img);


		gp << "set xrange[0:450]\n";
		gp << "set yrange[0:610]\n";
		gp.draw(multiplot);
		gp.flush();

		sleep(1000);


	}


}

#endif
