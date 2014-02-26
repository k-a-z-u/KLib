/*
 * TestNumOptVector.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: Frank Ebner
 */

#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/optimization/NumOptVector.h"

namespace K {

	TEST(NumericalOptimizationVector, init) {

		NumOptVector<8> data;
		for (unsigned int i = 0; i < 8; ++i) {ASSERT_EQ(0, data[i]);}

		NumOptVector<4> data2;
		data2[0] = 0;
		data2[1] = 1;
		data2[2] = 2;
		data2[3] = 3;
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, data2[i]);}

	}

	TEST(NumericalOptimizationVector, assign) {

		NumOptVector<4> vec1;
		vec1[0] = 0;
		vec1[1] = 1;
		vec1[2] = 2;
		vec1[3] = 3;
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, vec1[i]);}

		NumOptVector<4> vec2 = vec1;
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, vec2[i]);}

		vec2[0] = 4;
		vec2[1] = 5;
		vec2[2] = 6;
		vec2[3] = 7;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i+4, vec2[i]);}
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, vec1[i]);}

	}

	TEST(NumericalOptimizationVector, add) {

		NumOptVector<4> vec1;
		vec1 += 1.5;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(1.5, vec1[i]);}

		NumOptVector<4> vec2 = vec1 + 3.3;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(1.5, vec1[i]);}
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(1.5+3.3, vec2[i]);}

		NumOptVector<4> tmp;
		tmp[0] = 0;
		tmp[1] = 1;
		tmp[2] = 2;
		tmp[3] = 3;
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, tmp[i]);}

		vec2 += tmp;
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i, tmp[i]);}
		ASSERT_EQ(1.5+3.3+0, vec2[0]);
		ASSERT_EQ(1.5+3.3+1, vec2[1]);
		ASSERT_EQ(1.5+3.3+2, vec2[2]);
		ASSERT_EQ(1.5+3.3+3, vec2[3]);

	}

	TEST(NumericalOptimizationVector, sub) {

		NumOptVector<4> vec1;
		vec1 -= 1.1;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(-1.1, vec1[i]);}

		NumOptVector<4> vec2 = vec1 - 0.5;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(-1.1, vec1[i]);}
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(-1.1-0.5, vec2[i]);}

		NumOptVector<4> tmp;
		tmp[0] = 1;
		tmp[1] = 2;
		tmp[2] = 5;
		tmp[3] = 9;

		vec2 -= tmp;
		ASSERT_EQ(-1.1-0.5-1, vec2[0]);
		ASSERT_EQ(-1.1-0.5-2, vec2[1]);
		ASSERT_EQ(-1.1-0.5-5, vec2[2]);
		ASSERT_EQ(-1.1-0.5-9, vec2[3]);

	}

	TEST(NumericalOptimizationVector, mul) {

		NumOptVector<4> vec1;
		vec1[0] = 0;
		vec1[1] = 1;
		vec1[2] = 2;
		vec1[3] = 3;
		vec1 *= 2.2;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i*2.2, vec1[i]);}

		NumOptVector<4> vec2 = vec1 * 1.5;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i*2.2, vec1[i]);}
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i*2.2*1.5, vec2[i]);}

	}

	TEST(NumericalOptimizationVector, div) {

		NumOptVector<4> vec1;
		vec1[0] = 0;
		vec1[1] = 1;
		vec1[2] = 2;
		vec1[3] = 3;
		vec1 /= 2.2;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i/2.2, vec1[i]);}

		NumOptVector<4> vec2 = vec1 / 0.5;

		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i/2.2, vec1[i]);}
		for (unsigned int i = 0; i < 4; ++i) {ASSERT_EQ(i/2.2/0.5, vec2[i]);}

	}

}

#endif
