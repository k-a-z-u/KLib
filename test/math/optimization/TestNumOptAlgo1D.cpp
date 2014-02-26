/*
 * TestNumOptAlgo1D.cpp
 *
 *  Created on: Sep 4, 2013
 *      Author: Frank Ebner
 */



#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/optimization/NumOptFunction.h"
#include "../../../math/optimization/NumOptAlgoNewton.h"
#include "../../../math/optimization/NumOptAlgoGoldenCut.h"

namespace K {

	// f(x) = x^2 - 4x
	static struct NOA1func1 : public NumOptFunction<1> {
		double getValue(const NumOptVector<1>& data) const override {return data[0]*data[0] - 4 * data[0];};
	} NOA1func1;

	// f(x) = x - log(x)
	static struct NOA1func2 : public NumOptFunction<1> {
		double getValue(const NumOptVector<1>& data) const override {return data[0] - log(data[0]);};
	} NOA1func2;

	// f(x) = 0.25 x^3 + x		minimum @ -1
	static struct NOA1func3 : public NumOptFunction<1> {
		double getValue(const NumOptVector<1>& data) const override {return 0.25 * pow(data[0], 4) + data[0];};
	} NOA1func3;


	TEST(NumericalOptimizationNewton, testSimple) {

		NumOptVector<1> start;

		NumOptAlgoNewton opt1( 0.01 );
		opt1.calculateOptimum(NOA1func1, start);
		EXPECT_NEAR(2.0, start[0], 0.000001);

		// approx from left to minimum
		start[0] = 0.01;
		NumOptAlgoNewton opt2( 0.001 );
		opt2.calculateOptimum(NOA1func2, start);
		EXPECT_NEAR(1.0, start[0], 0.000001);

		// approx from right to minimum
		// function is NOT continous! (log(x)) -> larger start values will fail
		start[0] = 1.5;
		NumOptAlgoNewton opt2b( 0.001 );
		opt2b.calculateOptimum(NOA1func2, start);
		EXPECT_NEAR(1.0, start[0], 0.000001);


		// test function 3

		// approx left -> mininum
		start[0] = -2.0;
		NumOptAlgoNewton opt3a( 0.001 );
		opt3a.calculateOptimum(NOA1func3, start);
		EXPECT_NEAR(-1.0, start[0], 0.000001);

		// approx left -> mininum
		start[0] = -3.0;
		NumOptAlgoNewton opt3b( 0.001 );
		opt3b.calculateOptimum(NOA1func3, start);
		EXPECT_NEAR(-1.0, start[0], 0.000001);

		// approx right -> mininum
		start[0] = 1.0;
		NumOptAlgoNewton opt3c( 0.001 );
		opt3c.calculateOptimum(NOA1func3, start);
		EXPECT_NEAR(-1.0, start[0], 0.000001);

		// approx right -> mininum
		start[0] = 2.0;
		NumOptAlgoNewton opt3d( 0.001 );
		opt3d.calculateOptimum(NOA1func3, start);
		EXPECT_NEAR(-1.0, start[0], 0.000001);

	}

	TEST(NumericalOptimizationGoldenCut, testSimple) {

		NumOptVector<1> dst;
		NumOptAlgoGoldenCut opt(-10, 10, 0.001);

		opt.calculateOptimum(NOA1func1, dst);
		EXPECT_NEAR(2, dst[0], 0.001);

		opt.calculateOptimum(NOA1func3, dst);
		EXPECT_NEAR(-1, dst[0], 0.001);

		// func2 is not continuous -> adjust min/max
		NumOptAlgoGoldenCut opt2(0, 10, 0.001);
		opt2.calculateOptimum(NOA1func2, dst);
		EXPECT_NEAR(1, dst[0], 0.001);

	//	// approx from left to minimum
	//	start = {0.01};
	//	NumOptAlgoNewton opt2( 0.001 );
	//	opt2.calculateOptimum(func2, start);
	//	EXPECT_FLOAT_EQ(1, start[0]);
	//
	//	// approx from right to minimum
	//	// function is NOT continous! (log(x)) -> larger start values will fail
	//	start = {1.5};
	//	NumOptAlgoNewton opt2b( 0.001 );
	//	opt2b.calculateOptimum(func2, start);
	//	EXPECT_FLOAT_EQ(1, start[0]);
	//
	//
	//	// test function 3
	//
	//	// approx left -> mininum
	//	start = {-2};
	//	NumOptAlgoNewton opt3a( 0.001 );
	//	opt3a.calculateOptimum(func3, start);
	//	EXPECT_FLOAT_EQ(-1, start[0]);
	//
	//	// approx left -> mininum
	//	start = {-3};
	//	NumOptAlgoNewton opt3b( 0.001 );
	//	opt3b.calculateOptimum(func3, start);
	//	EXPECT_FLOAT_EQ(-1, start[0]);
	//
	//	// approx right -> mininum
	//	start = {1.0};
	//	NumOptAlgoNewton opt3c( 0.001 );
	//	opt3c.calculateOptimum(func3, start);
	//	EXPECT_FLOAT_EQ(-1, start[0]);
	//
	//	// approx right -> mininum
	//	start = {2.0};
	//	NumOptAlgoNewton opt3d( 0.001 );
	//	opt3d.calculateOptimum(func3, start);
	//	EXPECT_FLOAT_EQ(-1, start[0]);

	}

}

#endif
