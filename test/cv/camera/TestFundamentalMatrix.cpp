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
#include "../../../cv/features/Simple.h"

#include <queue>

namespace K {

	Eigen::Vector3d toVec(const Point2f p) {return Eigen::Vector3d(p.x, p.y, 1);}



	//StereoPlot plot(fileLeft, fileRight);

	struct Desc {
		std::string fileLeft;
		std::string fileRight;
		ImageChannel imgLeft;
		ImageChannel imgRight;
		FundamentalMatrix fm;
	};

	class StereoPlot {

	public:

		Gnuplot gp;
		GnuplotMultiplot multiplot;

		GnuplotPlot plotL;
		GnuplotPlotElementPoints lPoints;
		GnuplotPlotElementLines lLines;
		GnuplotPlotElementImage lImg;

		GnuplotPlot plotR;
		GnuplotPlotElementPoints rPoints;
		GnuplotPlotElementLines rLines;
		GnuplotPlotElementImage rImg;

	public:

		StereoPlot(Desc& desc) : multiplot(1,2), lImg(desc.fileLeft), rImg(desc.fileRight) {

			lPoints.setColorHex("#999900"); lPoints.setPointSize(0.6f); lPoints.setPointType(7);
			lLines.setColorHex("#0000ff");
			plotL.add(&lImg);
			plotL.add(&lLines);
			plotL.add(&lPoints);

			rPoints.setColorHex("#999900"); rPoints.setPointSize(0.6f); rPoints.setPointType(7);
			rLines.setColorHex("#0000ff");
			plotR.add(&rImg);
			plotR.add(&rLines);
			plotR.add(&rPoints);

			multiplot.add(&plotL);
			multiplot.add(&plotR);

			gp << "set terminal qt size 1800,800\n";
			gp << "set size ratio -1\n";
		//	gp << "set xrange[0:"<<desc.imgLeft.getWidth()<<"]\n";
		//	gp << "set yrange[0:"<<desc.imgRight.getHeight()<<"]\n";

		}

		void draw() {
			gp.draw(multiplot);
			gp.flush();
		}

	};



	struct Garden : public Desc {
		Garden() {

			fileLeft = getDataFile("stereo1.jpg");		// left image
			fileRight = getDataFile("stereo2.jpg");		// right image
			imgLeft = ImageFactory::readJPEG(fileLeft);
			imgRight = ImageFactory::readJPEG(fileRight);

			MatchingSAD sad(imgLeft, imgRight, 15);
			Matching matcher;

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
//			fm.addCorrespondence(pl9,	pr9);
//			fm.addCorrespondence(pl10,	pr10);

			fm.estimate();

		}
	};

	struct IDIS : public Desc {

		IDIS() {

			fileLeft = "/apps/workspaces/kiste_data/stereo/2/left.png";
			fileRight = "/apps/workspaces/kiste_data/stereo/2/right.png";
			imgLeft = ImageFactory::readPNG(fileLeft);
			imgRight = ImageFactory::readPNG(fileRight);

			MatchingSAD sad(imgLeft, imgRight, 15);
			Matching matcher;

			Point2i pl1(257,369);	Point2i pr1(98,367);		pr1 = matcher.refine(sad, pl1, pr1);
			Point2i pl2(261,579);	Point2i pr2(101,589);		pr2 = matcher.refine(sad, pl2, pr2);
			Point2i pl3(282,692);	Point2i pr3(121,708);		pr3 = matcher.refine(sad, pl3, pr3);
			Point2i pl4(657,673);	Point2i pr4(520,678);		pr4 = matcher.refine(sad, pl4, pr4);
			Point2i pl5(704,247);	Point2i pr5(569,247);		pr5 = matcher.refine(sad, pl5, pr5);
			Point2i pl6(816,641);	Point2i pr6(679,641);		pr6 = matcher.refine(sad, pl6, pr6);
			Point2i pl7(1113,503);	Point2i pr7(964,501);		pr7 = matcher.refine(sad, pl7, pr7);
			Point2i pl8(1025,472);	Point2i pr8(882,472);		pr8 = matcher.refine(sad, pl8, pr8);
			Point2i pl9(1096,345);	Point2i pr9(948,348);		pr9 = matcher.refine(sad, pl9, pr9);

			fm.addCorrespondence(pl1,	pr1);
			fm.addCorrespondence(pl2,	pr2);
			fm.addCorrespondence(pl3,	pr3);
			fm.addCorrespondence(pl4,	pr4);
			fm.addCorrespondence(pl5,	pr5);
			fm.addCorrespondence(pl6,	pr6);
			fm.addCorrespondence(pl7,	pr7);
			fm.addCorrespondence(pl8,	pr8);
			//fm.addCorrespondence(pl9,	pr9);

			fm.estimate();

		}



	};


	class StereoReconstruction {

	private:

		Desc& desc;

	public:

		/** ctor */
		StereoReconstruction(Desc& desc) : desc(desc) {
			;
		}

		void perform() {


			ImageChannel imgDepth(desc.imgLeft.getWidth(), desc.imgLeft.getHeight());
			imgDepth.ones();

			DataMatrix<SimpleFeatures::FeatureVec> fLeft = SimpleFeatures::getFeatures(desc.imgLeft, 15);
			DataMatrix<SimpleFeatures::FeatureVec> fRight = SimpleFeatures::getFeatures(desc.imgRight, 15);

			Gnuplot gp;
			GnuplotPlot p;
			GnuplotPlotElementLines errLine;
			p.add(&errLine);


//#pragma omp parallel 0
			for (int y = 0; y < desc.imgLeft.getHeight()-1; y+=2) {
				std::cout << y << std::endl;
				for (int x = 0; x < desc.imgLeft.getWidth()-1; x+=2) {


					const Point2i pL(x,y);

//					std::cout << "sigma: " << fLeft.get(pL.x, pL.y).sigma << std::endl;
//					std::cout << "asigma: " << fLeft.get(pL.x, pL.y).avgSigma << std::endl;
//					std::cout << "agradx: " << fLeft.get(pL.x, pL.y).avgGradX << std::endl;
//					std::cout << "agrady: " << fLeft.get(pL.x, pL.y).avgGradY << std::endl;

					const Eigen::Vector3d l = desc.fm.getEpilineRight(pL);
					const int x1 = 0;
					const int x2 = desc.imgLeft.getWidth();
					const int y1 = -(l(0)*x1 + l(2)) / l(1);
					const int y2 = -(l(0)*x2 + l(2)) / l(1);

					BresenhamIter iter(x1,y1,x2,y2);

					//Point2i pMin(0,0);
					//float vMin = INFINITY;



					struct QueueElem {
						float err;
						Point2i p;
						QueueElem(const float err, const Point2i p) : err(err), p(p) {;}
						QueueElem() : err(99999999), p(0,0) {;}
					};
					auto comp = [] (const QueueElem& a, const QueueElem& b) {return a.err < b.err;};
					std::priority_queue<QueueElem, std::vector<QueueElem>, decltype(comp)> queue(comp);

					// follow the epipolar line
					while (iter.hasNext()) {

						const Point2i pR = iter.next();

						const int maxD = desc.imgLeft.getWidth() / 4;
						if(pL.getDistance(pR) > maxD) {continue;}
						if(pL.getDistance(pR) < 10) {continue;}

						//const int w = 0;
						//for (int oy = -w; oy <= +w; ++oy) {
						//	for (int ox = -w; ox <= +w; ++ox) {
								//const Point2i pR = _pR + (Point2i(ox, oy));
								if (pR.x < 2 || pR.y < 2 || pR.x >= desc.imgRight.getWidth() - 2 || pR.y >= desc.imgRight.getHeight() - 2) {continue;}
								float err = fLeft.get(pL.x, pL.y).diff(fRight.get(pR.x, pR.y));
								//if (err < vMin) {vMin = err; pMin = pR;}
								queue.push(QueueElem(err, pR));
								//errLine.add( GnuplotPoint2(pR.x, err) );
								//std::cout << pR.x << "," << pR.y << std::endl;
						//	}
						//}

					}

//					gp.draw(p);
//					gp.flush();
//					usleep(1000*1000);

					if (!queue.empty()) {
						QueueElem bestInRight = queue.top();
						const float depth = pL.getDistance(bestInRight.p);
						imgDepth.set(pL.x, pL.y, depth);
						imgDepth.set(pL.x+1, pL.y, depth);
						imgDepth.set(pL.x, pL.y+1, depth);
						imgDepth.set(pL.x+1, pL.y+1, depth);
					}

//					if (vMin != INFINITY ) {
//						const float depth = pL.getDistance(bestInRight.p);
//						imgDepth.set(pL.x, pL.y, depth);
//						imgDepth.set(pL.x+1, pL.y, depth);
//						imgDepth.set(pL.x, pL.y+1, depth);
//						imgDepth.set(pL.x+1, pL.y+1, depth);
//					} else {
//						int i = 0; (void) i;
//					}






				}
			}

			Normalize::inplace(imgDepth);
			ImageFactory::writePNG("/tmp/depth.png", imgDepth);


		}

	};


	TEST(FundamentalMatrix, estimate) {

		Garden desc;

		StereoPlot plot(desc);

		StereoReconstruction sr(desc);
		sr.perform();


		for (int i = 0; i < 8; ++i) {

			const Eigen::Vector3d pil = toVec(desc.fm.getPointLeft(i));
			const Eigen::Vector3d pir = toVec(desc.fm.getPointRight(i));
			const float res = (pir.transpose() * (desc.fm.getFundamentalMatrix() * pil));
			std::cout << "#" << res << std::endl;
			//ASSERT_GE(0.1, std::abs(res));
		}

		for (int i = 0; i < 9; ++i) {
			//	const int i = 1;

			const Eigen::Vector3d piL = toVec(desc.fm.getPointLeft(i));
			const Eigen::Vector3d piR = toVec(desc.fm.getPointRight(i));

			const Eigen::Vector3d lR = desc.fm.getEpilineRight(piL);
			const Eigen::Vector3d lL = desc.fm.getEpilineLeft(piR);

			plot.lPoints.add(GnuplotPoint2(piL(0), piL(1)));
			plot.rPoints.add(GnuplotPoint2(piR(0), piR(1)));

			for (int xx = -400; xx < desc.imgLeft.getWidth()*3; xx+=25) {
				int x1 = xx;
				int x2 = xx + 25;

				{
					float y1 = -(lL(0)*x1 + lL(2)) / lL(1);
					float y2 = -(lL(0)*x2 + lL(2)) / lL(1);
					plot.lLines.addSegment(GnuplotPoint2(x1,y1), GnuplotPoint2(x2,y2));
				}

				{
					float y1 = -(lR(0)*x1 + lR(2)) / lR(1);
					float y2 = -(lR(0)*x2 + lR(2)) / lR(1);
					plot.rLines.addSegment(GnuplotPoint2(x1,y1), GnuplotPoint2(x2,y2));
				}

			}




			//}

		}

//		ImageFactory::writeJPEG("/tmp/1.jpg", img);



		plot.draw();

		sleep(1000);


	}


	}

#endif
