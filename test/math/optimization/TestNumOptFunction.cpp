/*
 * TestNumOptFunction.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS


#include "../../Test.h"

#include "../../../math/optimization/NumOptFunction.h"

namespace K {

	TEST(NumericalOptimizationFunction, derive) {

		struct : public NumOptFunction<1> {
			double getValue(const NumOptVector<1>& data) const override {return data[0]*data[0] - 4 * data[0];};
		} function;

		double maxDelta = 0.00005;

		NumOptVector<1> x;
		x[0] = 0;
		ASSERT_NEAR(-4, function.getF1(x), maxDelta);
		ASSERT_NEAR(2, function.getF2(x), maxDelta);

		x[0] = 1.5;
		ASSERT_NEAR(-1, function.getF1(x), maxDelta);
		ASSERT_NEAR(2, function.getF2(x), maxDelta);

		x[0] = 2.0;
		ASSERT_NEAR(0, function.getF1(x), maxDelta);
		ASSERT_NEAR(2, function.getF2(x), maxDelta);

		x[0] = 4.0;
		ASSERT_NEAR(4, function.getF1(x), maxDelta);
		ASSERT_NEAR(2, function.getF2(x), maxDelta);

	}

	TEST(NumericalOptimizationFunction, gradient) {

		struct Test1 : public NumOptFunction<1> {
			double getValue(const NumOptVector<1>& args) const {(void) args; return 1;}			// f(x) = 1;
		};

		struct Test2 : public NumOptFunction<1> {
			double getValue(const NumOptVector<1>& args) const {return args[0];}	// f(x) = x;
		};

		struct Test3 : public NumOptFunction<1> {
			double getValue(const NumOptVector<1>& args) const {return args[0]*args[0];}	// f(x) = x^2;
		};


		NumOptVector<1> tmpGrad;

		// check f(x) = 1
		Test1 f1;
		NumOptVector<1> v1;
		v1[0] = 0;
		f1.getGradient(v1, tmpGrad);
		ASSERT_EQ(0, tmpGrad[0]);

		// check f(x) = x
		Test2 f2;
		NumOptVector<1> v2;
		v2[0] = 0;
		f2.getGradient(v2, tmpGrad);
		ASSERT_EQ(1, tmpGrad[0]);

		// check f(x) = x^2
		Test3 f3;
		NumOptVector<1> v3;
		v3[0] = 0;
		f3.getGradient(v3, tmpGrad);
		ASSERT_NEAR(0, tmpGrad[0], 0.001);

		v3[0] = 1;
		f3.getGradient(v3, tmpGrad);
		ASSERT_NEAR(2, tmpGrad[0], 0.001);

		v3[0] = -1;
		f3.getGradient(v3, tmpGrad);
		ASSERT_NEAR(-2, tmpGrad[0], 0.001);

	}

}

#endif
