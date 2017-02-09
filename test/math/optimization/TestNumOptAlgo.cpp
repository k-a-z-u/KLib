/*
 * TestNumOptGradient.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/optimization/NumOptFunction.h"
#include "../../../math/optimization/NumOptAlgoNewton.h"
#include "../../../math/optimization/NumOptAlgoGoldenCut.h"
#include "../../../math/optimization/NumOptAlgoGradient.h"
#include "../../../math/optimization/NumOptAlgoHillClimb.h"
#include "../../../math/optimization/NumOptAlgoDownhillSimplex.h"

namespace K {

	/**
	 * test all optimization algorithms using some default testing-functions
	 * like himmelblau or rosenbrock
	 */


	static struct TNOAfunc1 : NumOptFunction<2> {
		double getValue(const NumOptVector<2>& data) const override {
			return data[0]*data[0] + (data[1]+1.0)*(data[1]+1.0);
		};
		double operator() (const double* data) const {
			return data[0]*data[0] + (data[1]+1.0)*(data[1]+1.0);
		};
	} TNOAfunc1;

	static struct TNOAfunc2 : NumOptFunction<2> {
		double getValue(const NumOptVector<2>& data) const override {
			return pow(data[0]-2,2) + pow(data[1]-4,2);
		};
		double operator() (const double* data) const {
			return pow(data[0]-2,2) + pow(data[1]-4,2);
		};
	} TNOAfunc2;

	static struct TNOAfunc3 : NumOptFunction<2> {
		double getValue(const NumOptVector<2>& data) const override {
			return 0.00025*pow(data[0],4) + data[0] + 0.1*pow(data[1]-4,2);
		};
		double operator() (const double* data) const {
			return 0.00025*pow(data[0],4) + data[0] + 0.1*pow(data[1]-4,2);
		};
	} TNOAfunc3;


	static struct TNOAfuncHimmelblau : NumOptFunction<2> {
		double getValue(const NumOptVector<2>& data) const override {
			return pow( (data[0]*data[0] + data[1] - 11), 2) + pow( (data[0]+data[1]*data[1]-7), 2);
		};
		double operator() (const double* data) const {
			return pow( (data[0]*data[0] + data[1] - 11), 2) + pow( (data[0]+data[1]*data[1]-7), 2);
		};
	} TNOAfuncHimmelblau;

	static struct TNOAfuncRosenbrock : NumOptFunction<2> {
		double getValue(const NumOptVector<2>& data) const override {
			return pow(1 - data[0], 2) + 100 * pow( (data[1] - data[0]*data[0]), 2);
		};
		double operator() (const double* data) const {
			return pow(1 - data[0], 2) + 100 * pow( (data[1] - data[0]*data[0]), 2);
		};
	} TNOAfuncRosenbrock;




	/** -------------------------------- HULL-CLIMB -------------------------------- */

	TEST(NumericalOptimizationAlgo, hillClimb1) {

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = -1;
		ASSERT_EQ(0, TNOAfunc1.getValue(data));

		data[0] = 2;
		data[1] = 2;

		ASSERT_EQ(2, data[0]);
		ASSERT_EQ(2, data[1]);
//		ASSERT_EQ(2, data.data[0]);
//		ASSERT_EQ(2, data.data[1]);

		NumOptAlgoHillClimb<2> opt(0.5);
		opt.calculateOptimum(TNOAfunc1, data);

		ASSERT_NEAR(0, data[0], 0.1);
		ASSERT_NEAR(-1, data[1], 0.1);



	}

	TEST(NumericalOptimizationAlgo, hillClimb2) {

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoHillClimb<2> opt(0.5);
		opt.calculateOptimum(TNOAfunc2, data);

		ASSERT_NEAR(2, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, hillClimb3) {

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoHillClimb<2> opt(0.5);
		opt.calculateOptimum(TNOAfunc3, data);

		ASSERT_NEAR(-10, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, hillClimbHimmelblau) {

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoHillClimb<2> opt(0.25);
		opt.calculateOptimum(TNOAfuncHimmelblau, data);

		ASSERT_NEAR(-2.805118, data[0], 0.01);
		ASSERT_NEAR(3.131312, data[1], 0.01);

	}

	TEST(NumericalOptimizationAlgo, hillClimbRosenbrock) {

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoHillClimb<2> opt(0.001);				// VERY SMALL!!!
		opt.calculateOptimum(TNOAfuncRosenbrock, data);

		ASSERT_NEAR(1, data[0], 0.02);					// VERY HIGH!
		ASSERT_NEAR(1, data[1], 0.02);

	}



	/** -------------------------------- GRADIENT -------------------------------- */


	TEST(NumericalOptimizationAlgo, gradient1) {

		NumOptAlgoNewton newton;
		NumOptVector<2> data;
		data[0] = 2;
		data[1] = 2;

		NumOptAlgoGradient<2> opt(newton);
		opt.calculateOptimum(TNOAfunc1, data);

		ASSERT_NEAR(0, data[0], 0.1);
		ASSERT_NEAR(-1, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, gradient2) {

		NumOptAlgoNewton newton;
		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoGradient<2> opt(newton);
		opt.calculateOptimum(TNOAfunc2, data);

		ASSERT_NEAR(2, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, gradient3) {

		NumOptAlgoNewton newton;

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoGradient<2> opt(newton);
		opt.calculateOptimum(TNOAfunc3, data);

		ASSERT_NEAR(-10, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, downhillGradientHimmelblau) {

		NumOptAlgoNewton newton;

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoGradient<2> opt(newton);
		opt.calculateOptimum(TNOAfuncHimmelblau, data);

		ASSERT_NEAR(3.0, data[0], 0.01);
		ASSERT_NEAR(2.0, data[1], 0.01);

	}

	TEST(NumericalOptimizationAlgo, downhillGradientRosenbrock) {

		NumOptAlgoNewton newton;

		NumOptVector<2> data;
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoGradient<2> opt(newton, 0.00001, 0.0001);
		opt.calculateOptimum(TNOAfuncRosenbrock, data);

		ASSERT_NEAR(1, data[0], 0.02);
		ASSERT_NEAR(1, data[1], 0.02);

	}


	/** -------------------------------- DOWNHILL SIMPLEX -------------------------------- */

	TEST(NumericalOptimizationAlgo, downhillSimplex1) {

		double data[2];
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoDownhillSimplex<double> opt(2);
		opt.calculateOptimum(TNOAfunc1, data);

		ASSERT_NEAR(0, data[0], 0.1);
		ASSERT_NEAR(-1, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, downhillSimplex2) {

		double data[2];
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoDownhillSimplex<double> opt(2);
		opt.setNumRestarts(2);
		opt.setMaxIterations(100);
		opt.calculateOptimum(TNOAfunc2, data);


		ASSERT_NEAR(2, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, downhillSimplex3) {

		double data[2];
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoDownhillSimplex<double> opt(2);
		opt.calculateOptimum(TNOAfunc3, data);

		ASSERT_NEAR(-10, data[0], 0.1);
		ASSERT_NEAR(4, data[1], 0.1);

	}

	TEST(NumericalOptimizationAlgo, downhillSimplexHimmelblau) {

		double data[2];
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoDownhillSimplex<double> opt(2);
		opt.calculateOptimum(TNOAfuncHimmelblau, data);

		// solution 1
		ASSERT_NEAR(3, data[0], 0.01);
		ASSERT_NEAR(2, data[1], 0.01);

//		// solution 4
//		ASSERT_NEAR(3.584428, data[0], 0.01);
//		ASSERT_NEAR(-1.848126, data[1], 0.01);

	}

	TEST(NumericalOptimizationAlgo, downhillSimplexRosenbrock) {

		double data[2];
		data[0] = 0;
		data[1] = 0;

		NumOptAlgoDownhillSimplex<double> opt(2);
		opt.setNumRestarts(2);
		opt.setMaxIterations(100);
		opt.calculateOptimum(TNOAfuncRosenbrock, data);

		ASSERT_NEAR(1, data[0], 0.01);
		ASSERT_NEAR(1, data[1], 0.01);

	}

}

#endif
