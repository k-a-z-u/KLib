/*
 * Windows.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/dsp/window/DSPWindowBlackman.h"
#include "../../../math/dsp/window/DSPWindowHanning.h"
#include "../../../math/dsp/window/DSPWindowHamming.h"
#include "../../../math/dsp/window/DSPWindowBartlett.h"

namespace K {

	/** fill array with simple sine function */
	void fillSine(double arr[], unsigned int len) {
		for (unsigned int i = 0; i < len; ++i) {
			arr[i] = sin(K::PI * i / 16.0);
		}
	}

	/** fill array with constant value (1.0) */
	void fillConst(double arr[], unsigned int len) {
		for (unsigned int i = 0; i < len; ++i) {
			arr[i] = 1.0;
		}
	}



	TEST(DSP, WindowBlackman) {

		double arr[512];
		fillConst(arr, 512);

		DSPWindowBlackman<double, 512> wb;
		wb.apply(arr);

		EXPECT_NEAR(0.0f, (float) arr[0], 0.0001);
		EXPECT_NEAR(1.0f, (float) arr[256], 0.0001);
		EXPECT_NEAR(0.0f, (float) arr[511], 0.0001);

	}

	TEST(DSP, WindowHamming) {

		double arr[512];
		fillConst(arr, 512);

		DSPWindowHamming<double, 512> wb;
		wb.apply(arr);

		EXPECT_NEAR(0.08f, (float) arr[0], 0.01);
		EXPECT_NEAR(1.0f, (float) arr[256], 0.0001);
		EXPECT_NEAR(0.08f, (float) arr[511], 0.01);

	}

	TEST(DSP, WindowHanning) {

		double arr[512];
		fillConst(arr, 512);

		DSPWindowHanning<double, 512> wb;
		wb.apply(arr);

		EXPECT_NEAR(0.0f, (float) arr[0], 0.0001);
		EXPECT_NEAR(1.0f, (float) arr[256], 0.0001);
		EXPECT_NEAR(0.0f, (float) arr[511], 0.0001);

	}

	TEST(DSP, WindowBartlett) {

		double arr[512];
		fillConst(arr, 512);

		DSPWindowBartlett<double, 512> wb;
		wb.apply(arr);

		EXPECT_NEAR(0.0f, (float) arr[0], 0.0001);
		EXPECT_NEAR(0.5f, (float) arr[128], 0.001);
		EXPECT_NEAR(1.0f, (float) arr[256], 0.002);
		EXPECT_NEAR(0.5f, (float) arr[383], 0.001);
		EXPECT_NEAR(0.0f, (float) arr[511], 0.0001);

	}

}

#endif
