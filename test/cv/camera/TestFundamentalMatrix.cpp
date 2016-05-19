#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/camera/FundamentalMatrix.h"
#include "../../../cv/camera/Homography.h"

#include "../../../cv/ImageFactory.h"
#include "../../../cv/draw/Drawer.h"

#include "../../../misc/gnuplot/Gnuplot.h"
#include "../../../misc/gnuplot/GnuplotPlot.h"
#include "../../../misc/gnuplot/GnuplotPlotElementPoints.h"
#include "../../../misc/gnuplot/GnuplotPlotElementLines.h"
#include "../../../misc/gnuplot/GnuplotPlotElementImage.h"
#include "../../../misc/gnuplot/GnuplotMultiplot.h"

#include "../../../misc/gnuplot/GnuplotSplot.h"
#include "../../../misc/gnuplot/GnuplotSplotElementPoints.h"

#include "../../../cv/matching/Matching.h"
#include "../../../cv/matching/MatchingSAD.h"
#include "../../../cv/matching/MatchingConvolution.h"

#include "../../../cv/draw/BresenhamIter.h"

#include "../../../cv/filter/Normalize.h"
#include "../../../cv/features/Simple.h"
#include "../../../cv/features/HOG.h"

#include "../../../cv/Derivative.h"
#include "../../../cv/filter/Normalize.h"

#include <queue>
#include <set>

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

			lPoints.setColorHex("#ff0000"); lPoints.setPointSize(0.8f); lPoints.setPointType(7);
			lLines.setColorHex("#0000ff");
			plotL.add(&lImg);
			plotL.add(&lLines);
			plotL.add(&lPoints);

			rPoints.setColorHex("#00AA00"); rPoints.setPointSize(0.8f); rPoints.setPointType(7);
			rLines.setColorHex("#0000ff");
			plotR.add(&rImg);
			plotR.add(&rLines);
			plotR.add(&rPoints);

			multiplot.add(&plotL);
			multiplot.add(&plotR);

			gp << "set terminal qt size 1800,800\n";
			gp << "set size ratio -1\n";
			gp << "set xrange[0:"<<desc.imgLeft.getWidth()<<"]\n";
			gp << "set yrange[0:"<<desc.imgRight.getHeight()<<"]\n";

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

	struct Metal1 : public Desc {

		Metal1() {

			fileLeft = "/apps/workspaces/kiste_data/stereo/3/left.png";
			fileRight = "/apps/workspaces/kiste_data/stereo/3/right.png";
			imgLeft = ImageFactory::readPNG(fileLeft);
			imgRight = ImageFactory::readPNG(fileRight);

			MatchingSAD sad(imgLeft, imgRight, 15);
			Matching matcher;

			Point2i pl1(608,834);	Point2i pr1(400,873);		//pr1 = matcher.refine(sad, pl1, pr1);
			Point2i pl2(357,485);	Point2i pr2(292,470);		//pr2 = matcher.refine(sad, pl2, pr2);
			Point2i pl3(461,672);	Point2i pr3(294,678);		//pr3 = matcher.refine(sad, pl3, pr3);
			Point2i pl4(524,507);	Point2i pr4(410,523);		//pr4 = matcher.refine(sad, pl4, pr4);
			Point2i pl5(518,479);	Point2i pr5(440,493);		//pr5 = matcher.refine(sad, pl5, pr5);
			Point2i pl6(586,405);	Point2i pr6(476,431);		//pr6 = matcher.refine(sad, pl6, pr6);
			Point2i pl7(593,324);	Point2i pr7(532,349);		//pr7 = matcher.refine(sad, pl7, pr7);
			Point2i pl8(673,366);	Point2i pr8(596,405);		//pr8 = matcher.refine(sad, pl8, pr8);

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

	struct House1 : public Desc {

		House1() {

			fileLeft = "/apps/workspaces/kiste_data/stereo/4/left.png";
			fileRight = "/apps/workspaces/kiste_data/stereo/4/right.png";
			imgLeft = ImageFactory::readPNG(fileLeft);
			imgRight = ImageFactory::readPNG(fileRight);

			MatchingSAD sad(imgLeft, imgRight, 15);
			Matching matcher;

			Point2i pl1(110,244);	Point2i pr1(198,296);		//pr1 = matcher.refine(sad, pl1, pr1);
			Point2i pl2(273,242);	Point2i pr2(309,249);		//pr2 = matcher.refine(sad, pl2, pr2);
			Point2i pl3(88,117);	Point2i pr3(129,121);		//pr3 = matcher.refine(sad, pl3, pr3);
			Point2i pl4(247,122);	Point2i pr4(244,89);		//pr4 = matcher.refine(sad, pl4, pr4);
			Point2i pl5(321,100);	Point2i pr5(320,50);		//pr5 = matcher.refine(sad, pl5, pr5);
			Point2i pl6(398,162);	Point2i pr6(428,125);		//pr6 = matcher.refine(sad, pl6, pr6);
			Point2i pl7(393,282);	Point2i pr7(424,273);		//pr7 = matcher.refine(sad, pl7, pr7);
			Point2i pl8(266,154);	Point2i pr8(278,132);		//pr8 = matcher.refine(sad, pl8, pr8);

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

	struct House3D : public Desc {

		House3D() {

			fileLeft = "/apps/workspaces/kiste_data/stereo/5/left.png";
			fileRight = "/apps/workspaces/kiste_data/stereo/5/right.png";
			imgLeft = ImageFactory::readPNG(fileLeft);
			imgRight = ImageFactory::readPNG(fileRight);


//			imgLeft = Derivative::getX(imgLeft);
//			imgRight = Derivative::getX(imgRight);

//			Normalize::inplace(imgLeft);
//			Normalize::inplace(imgRight);

			MatchingSAD sad(imgLeft, imgRight, 15);
			Matching matcher;

			Point2i pl1(245,381);	Point2i pr1(176,366);		//pr1 = matcher.refine(sad, pl1, pr1);		// h1 lower left
			Point2i pl2(308,323);	Point2i pr2(312,323);		//pr2 = matcher.refine(sad, pl2, pr2);		// h1 lower right
			Point2i pl3(222,82);	Point2i pr3(246,82);		//pr3 = matcher.refine(sad, pl3, pr3);		// h1 upper right
			Point2i pl4(134,76);	Point2i pr4(101,78);		//pr4 = matcher.refine(sad, pl4, pr4);		// h1 upper left
			Point2i pl5(501,218);	Point2i pr5(416,228);		//pr5 = matcher.refine(sad, pl5, pr5);		// h1 mid left
			Point2i pl6(620,201);	Point2i pr6(625,219);		//pr6 = matcher.refine(sad, pl6, pr6);		// h1 mid right
			Point2i pl7(546,83);	Point2i pr7(570,78);		//pr7 = matcher.refine(sad, pl7, pr7);		// h1 upper right
			Point2i pl8(423,79);	Point2i pr8(378,77);		//pr8 = matcher.refine(sad, pl8, pr8);		// h1 upper left

			Point2i pl9(381,413);	Point2i pr9(319,420);		//pr8 = matcher.refine(sad, pl8, pr8);		// cube1a
			Point2i plA(153,322);	Point2i prA(68,299);		//pr8 = matcher.refine(sad, pl8, pr8);		// cube2
			Point2i plB(348,361);	Point2i prB(294,362);		//pr8 = matcher.refine(sad, pl8, pr8);		// cube1b


//			Point2i pl1(242,219);	Point2i pr1(171,212);		//pr1 = matcher.refine(sad, pl1, pr1);		// h1 lower left
//			Point2i pl2(307,194);	Point2i pr2(311,194);		//pr2 = matcher.refine(sad, pl2, pr2);		// h1 lower right
//			Point2i pl3(222,82);	Point2i pr3(246,82);		//pr3 = matcher.refine(sad, pl3, pr3);		// h1 upper right
//			Point2i pl4(134,76);	Point2i pr4(101,78);		//pr4 = matcher.refine(sad, pl4, pr4);		// h1 upper left
//			Point2i pl5(501,218);	Point2i pr5(416,228);		//pr5 = matcher.refine(sad, pl5, pr5);		// h1 lower left
//			Point2i pl6(620,201);	Point2i pr6(625,219);		//pr6 = matcher.refine(sad, pl6, pr6);		// h1 lower right
//			Point2i pl7(546,83);	Point2i pr7(570,78);		//pr7 = matcher.refine(sad, pl7, pr7);		// h1 upper right
//			Point2i pl8(423,79);	Point2i pr8(378,77);		//pr8 = matcher.refine(sad, pl8, pr8);		// h1 upper left

			fm.addCorrespondence(pl1,	pr1);
			fm.addCorrespondence(pl2,	pr2);
			fm.addCorrespondence(pl3,	pr3);
			fm.addCorrespondence(pl4,	pr4);
			fm.addCorrespondence(pl5,	pr5);
			fm.addCorrespondence(pl6,	pr6);
			fm.addCorrespondence(pl7,	pr7);
			fm.addCorrespondence(pl8,	pr8);
			fm.addCorrespondence(pl9,	pr9);

			fm.addCorrespondence(plA,	prA);
			fm.addCorrespondence(plB,	prB);

			fm.estimate();

		}

	};



	class StereoReconstruction {

	private:

		Desc& desc;

	public:

		/** ctor */
		StereoReconstruction(Desc& desc) : desc(desc) {

//			ImageChannel img(600,600);
//			img.ones();
//			Drawer d(img);


//			for (int i = 0; i < 9; ++i) {
//				const Eigen::Vector3d a = toVec(fm.getImg1(i));
//	//			const Eigen::Vector3d b = toVec(fm.getImg2(i));
//	//			const float res = (a.transpose() * (fm.get() * b));
//	//			std::cout << "#" << res << std::endl;
//	//			ASSERT_GE(0.1, std::abs(res));
//				Eigen::Vector3d l = fm.get() * a;
//				std::cout << "#" << l << std::endl;
//				l /= l(2);
//				//l *= 50;

//				Eigen::Vector3d s = a*20;
//				d.drawLine(l(0), l(1), l(0)*100, l(1)*100);
//				ImageFactory::writeJPEG("/tmp/1.jpg", img);


//			}

		}

		void perform() {

			ImageChannel imgDepth(desc.imgLeft.getWidth(), desc.imgLeft.getHeight());
			imgDepth.ones();

			Gauss g(1.0);
			desc.imgLeft = g.filter(desc.imgLeft);
			desc.imgRight = g.filter(desc.imgRight);

			MatchingSAD sad(desc.imgLeft, desc.imgRight, 13);
			MatchingConvolution conv(desc.imgLeft, desc.imgRight, 9);
			Matching matcher;

			DataMatrix<SimpleFeatures::FeatureVec> fLeft = SimpleFeatures::getFeatures(desc.imgLeft, 25);
			DataMatrix<SimpleFeatures::FeatureVec> fRight = SimpleFeatures::getFeatures(desc.imgRight, 25);

			Gnuplot gp;
			GnuplotPlot p;
			GnuplotPlotElementLines errLine;
			p.add(&errLine);

			GnuplotSplot splot;
			GnuplotSplotElementPoints depthPlot;
			splot.add(&depthPlot);


			DataMatrix<std::vector<float>> hogLeft(desc.imgLeft.getWidth(), desc.imgRight.getHeight());
			DataMatrix<std::vector<float>> hogRight(desc.imgLeft.getWidth(), desc.imgRight.getHeight());
			HOG hog1(desc.imgLeft, 10);
			HOG hog2(desc.imgRight, 10);

//			for (int y = 0; y < desc.imgLeft.getHeight(); ++y) {
//				for (int x = 0; x < desc.imgLeft.getWidth(); ++x) {


//					std::vector<HOGGradient> gLeft =	hog1.getGradients(desc.imgLeft, x, y);
//					std::vector<HOGGradient> gRight =	hog2.getGradients(desc.imgRight, x, y);
//					hog.relativeToBestOne(gLeft);
//					hog.relativeToBestOne(gRight);
//					hog.normalize(gLeft);
//					hog.normalize(gRight);
//					hogLeft.set(x,y,hog.binify(gLeft, 8));
//					hogRight.set(x,y,hog.binify(gRight, 8));

//				}
//			}



			struct QueueElem {
				float err;
				Point2i p;
				QueueElem(const float err, const Point2i p) : err(err), p(p) {;}
				QueueElem() : err(99999999), p(0,0) {;}
				bool operator < (const QueueElem& o) const {return err > o.err;}
			};

//			auto comp = [] (const QueueElem& a, const QueueElem& b) {
//				return a.err < b.err;
//			};

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



					std::priority_queue<QueueElem, std::vector<QueueElem>> queue;


					// follow the epipolar line
					while (iter.hasNext()) {

						const Point2i _pR = iter.next();

						const int maxD = desc.imgLeft.getWidth() / 4;
						if(pL.getDistance(_pR) > maxD) {continue;}
						//if(pL.getDistance(pR) < 10) {continue;}

						//const int w = 0;
						for (int oy = -1; oy <= +1; ++oy) {
						//	for (int ox = -w; ox <= +w; ++ox) {
								const Point2i pR = _pR + (Point2i(0, oy));
								if (pR.x < 2 || pR.y < 2 || pR.x >= desc.imgRight.getWidth() - 2 || pR.y >= desc.imgRight.getHeight() - 2) {continue;}
								//float err = - fLeft.get(pL.x, pL.y).diff(fRight.get(pR.x, pR.y));
								float err = sad.getError(pL, pR);
								//float err = -conv.getScore(pL, pR);
								//float err = HOG::getDistance( hogLeft.get(pL.x, pL.y), hogRight.get(pR.x, pR.y) );

								//if (err < vMin) {vMin = err; pMin = pR;}
								queue.push(QueueElem(err, pR));
								//errLine.add( GnuplotPoint2(pR.x, err) );
								//std::cout << pR.x << "," << pR.y << std::endl;
						//	}
						}

					}

//					gp.draw(p);
//					gp.flush();
//					usleep(1000*1000);



					if (queue.size() > 10) {

						Point2i best = queue.top().p;

						int cnt = 8;
						Point2i sum(0,0);
						for (int i = 0; i < cnt; ++i) {
							sum += queue.top().p;
							queue.pop();
						}
						sum /= cnt;

						//if (best.getDistance(sum) < 25) {

							const float depth = pL.getDistance(best);
							imgDepth.set(pL.x, pL.y, depth);
							imgDepth.set(pL.x+1, pL.y, depth);
							imgDepth.set(pL.x, pL.y+1, depth);
							imgDepth.set(pL.x+1, pL.y+1, depth);

							depthPlot.add(GnuplotPoint3(best.x, best.y, depth));

						//}

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

			gp.draw(splot);
			gp.flush();

			Normalize::inplace(imgDepth);
			ImageFactory::writePNG("/tmp/depth.png", imgDepth);


		}



	};


	TEST(FundamentalMatrix, estimate) {

		Garden desc;
		//IDIS desc;
		//Metal1 desc;
		//House3D desc;


		StereoPlot plot(desc);

		StereoReconstruction sr(desc);
		//sr.perform();


		for (int i = 0; i < 8; ++i) {

			const Eigen::Vector3d pil = toVec(desc.fm.getPointLeft(i));
			const Eigen::Vector3d pir = toVec(desc.fm.getPointRight(i));
			const float res = (pir.transpose() * (desc.fm.getFundamentalMatrix() * pil));
			std::cout << "#" << res << std::endl;
			//ASSERT_GE(0.1, std::abs(res));
		}

		plot.rPoints.add(GnuplotPoint2( desc.fm.getEpipoleRight()(0), desc.fm.getEpipoleRight()(1) ));
		plot.lPoints.add(GnuplotPoint2( desc.fm.getEpipoleLeft()(0), desc.fm.getEpipoleLeft()(1) ));

		for (int i = 0; i < desc.fm.getNumCorrespondences(); ++i) {
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

		ImageChannel test(desc.imgRight.getWidth(), desc.imgRight.getHeight());
		Transform::affine(desc.fm.getRectificationRight(), desc.imgRight, test);

		ImageFactory::writeJPEG("/tmp/1.jpg", test);



		plot.draw();

		sleep(1000);


	}


	}

#endif
