/*
 * main.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */


#include "test/Test.h"

int main(int argc, char** argv) {

#ifdef WITH_TESTS
	//::testing::GTEST_FLAG(filter) = "*FixedPool*";
	//::testing::GTEST_FLAG(filter) = "*Vector*";
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif


}
