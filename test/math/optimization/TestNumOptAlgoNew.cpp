#ifdef WITH_TESTS

#include "../../Test.h"

#include "NumOptFunctions.h"
#include "../../../math/optimization/NumOptFunction.h"
#include "../../../math/optimization/NumOptAlgoNewton.h"
#include "../../../math/optimization/NumOptAlgoGoldenCut.h"
#include "../../../math/optimization/NumOptAlgoGradient.h"
#include "../../../math/optimization/NumOptAlgoHillClimb.h"
#include "../../../math/optimization/NumOptAlgoDownhillSimplex.h"
#include "../../../math/optimization/NumOptAlgoGenetic.h"
#include "../../../math/optimization/NumOptSimulatedAnnealing.h"
#include "../../../math/optimization/NumOptAlgoBestSet.h"

#include "../../../misc/gnuplot/Gnuplot.h"
#include "../../../misc/gnuplot/GnuplotPlot.h"
#include "../../../misc/gnuplot/GnuplotPlotElementLines.h"
#include "../../../misc/gnuplot/GnuplotSplot.h"
#include "../../../misc/gnuplot/GnuplotSplotElementLines.h"
#include "../../../misc/gnuplot/GnuplotSplotElementMesh.h"

#include <functional>

namespace K {

	struct Plot {

		Gnuplot gp;
		GnuplotPlot plot;
		GnuplotPlotElementLines fu;
		GnuplotPlotElementLines bar;

		template <typename Func> Plot(Func func, float min = -10, float max = +10) {
			plot.add(&fu);
			plot.add(&bar);
			float stepSize = (max-min) / 300;
			for (float x = min; x <= max; x+=stepSize) {
				float y = func(&x);
				fu.add(GnuplotPoint2(x,y));
			}
		}

		void show (const int iter, const float err, const float* val) {
			bar.clear();
			bar.addSegment(GnuplotPoint2(*val, -5), GnuplotPoint2(*val, +5));
			gp.draw(plot);
			gp.flush();
			usleep(1000*250);
		}

		std::function<void(int, float, const float*)> get() {
			return std::bind(&Plot::show, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}

	};

	struct Plot2 {

		Gnuplot gp;
		GnuplotSplot splot;
		GnuplotSplotElementMesh mesh;
		float minZ = +99999;
		float maxZ = -99999;

		template <typename Func> Plot2(Func func, float min = -10, float max = +10, const int cnt = 100) {
			splot.add(&mesh);
			//plot.add(&bar);
			float stepSize = (max-min) / cnt;

			struct XY {
				float x;
				float y;
			} xy;

			for (xy.y = min; xy.y <= max; xy.y+=stepSize) {
				for (xy.x = min; xy.x <= max; xy.x+=stepSize) {
					float z = func((float*)&xy);
					if (z < minZ) {minZ = z;}
					if (z > maxZ) {maxZ = z;}
					mesh.add(GnuplotPoint3(xy.x, xy.y, z));
				}
			}
		}

		void show (const int iter, const float err, const float* val) {
//			bar.clear();
//			bar.addSegment(GnuplotPoint2(*val, -5), GnuplotPoint2(*val, +5));
			gp.draw(splot);
			const float x = val[0];
			const float y = val[1];
			gp << "set arrow 1 from " << x << "," << y << "," << minZ << " to " << x << "," << y << "," << maxZ << "\n";
			gp.flush();
			usleep(1000*250);
		}

		std::function<void(int, float, const float*)> get() {
			return std::bind(&Plot2::show, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}

	};





//	/** -------------------------------- DOWNHILL SIMPLEX -------------------------------- */

//	TEST(NumOptNew, downhillSimplexNoise1) {

//		float data[1] = {2};

//		NumOptAlgoDownhillSimplex<float, 1> opt;
//		opt.calculateOptimum(Opt::fNoise1, data);

//		ASSERT_NEAR(0, data[0], 0.1);

//	}

//	TEST(NumOptNew, downhillSimplexNoise2) {

//		float data[1] = {2};

//		NumOptAlgoDownhillSimplex<float, 1> opt;
//		opt.calculateOptimum(Opt::fNoise2, data);

//		ASSERT_NEAR(0, data[0], 0.1);

//	}

//	/** -------------------------------- GENETIC -------------------------------- */

//	TEST(NumOptNew, geneticNoise1) {
//		float data[1] = {2};
//		NumOptAlgoGenetic<float> opt(1);
//		opt.calculateOptimum(Opt::fNoise1, data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}

//	TEST(NumOptNew, geneticNoise2) {
//		float data[1] = {2};
//		NumOptAlgoGenetic<float> opt(1);
//		opt.calculateOptimum(Opt::fNoise2, data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}


	/** -------------------------------- BEST SET -------------------------------- */
//	TEST(NumOptNew, BestSetSimple) {
//		float data[1] = {8};
//		NumOptAlgoBestSet<float, 1> opt(200);
//		Plot plt(Opt::fSimple);
//		opt.setCallback(plt.get());
//		opt.findMinimum(Opt::fSimple, NumOptAlgoBestSet<float,1>::ModifyAll(4,0), data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}
//	TEST(NumOptNew, BestSetNoise0) {
//		float data[1] = {8};
//		NumOptAlgoBestSet<float, 1> opt(200);
//		Plot plt(Opt::fNoise0);
//		opt.setCallback(plt.get());
//		opt.findMinimum(Opt::fNoise0, NumOptAlgoBestSet<float,1>::ModifyAll(4,0), data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}
//	TEST(NumOptNew, BestSetNoise1) {
//		float data[1] = {8};
//		NumOptAlgoBestSet<float, 1> opt(200);
//		Plot plt(Opt::fNoise1);
//		opt.setCallback(plt.get());
//		opt.findMinimum(Opt::fNoise1, NumOptAlgoBestSet<float,1>::ModifyAll(4,0), data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}
//	TEST(NumOptNew, BestSetNoise2) {
//		float data[1] = {8};
//		NumOptAlgoBestSet<float, 1> opt(200);
//		Plot plt(Opt::fNoise2);
//		opt.setCallback(plt.get());
//		opt.findMinimum(Opt::fNoise2, NumOptAlgoBestSet<float,1>::ModifyAll(4,0), data);
//		ASSERT_NEAR(0, data[0], 0.1);
//	}
//	TEST(NumOptNew, BestSetRosenbrock) {
//		float data[2] = {-1};
//		NumOptAlgoBestSet<float, 2> opt(200);
//		Plot2 plt(Opt::fRosenbrock, -2, 2, 50);
//		opt.setCallback(plt.get());
//		opt.findMinimum(Opt::fRosenbrock, NumOptAlgoBestSet<float,2>::ModifyAll(1,0), data);
//		ASSERT_NEAR(1, data[0], 0.1);
//		ASSERT_NEAR(1, data[1], 0.1);
//	}
	TEST(NumOptNew, BestSetRosenbrock) {
		float data[2] = {-1};
		NumOptAlgoBestSet<float, 2> opt(200);
		Plot2 plt(Opt::fHimmelblau, -4, 4, 50);
		opt.setCallback(plt.get());
		opt.findMinimum(Opt::fHimmelblau, NumOptAlgoBestSet<float,2>::ModifyAll(1,0), data);
//		ASSERT_NEAR(1, data[0], 0.1);
//		ASSERT_NEAR(1, data[1], 0.1);
	}


//	/** -------------------------------- SIMULATED ANNEALING -------------------------------- */



//	TEST(NumOptNew, SANoise1) {
//		float data[1] = {2};

//		NumOptAlgoSimulatedAnnealing<float, 1> opt(1000);
//		opt.setCallback(plotCallback);
//		opt.calculateOptimum(Opt::fNoise0, data);

//		ASSERT_NEAR(0, data[0], 0.1);
//	}





}

#endif
