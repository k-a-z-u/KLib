/*
 * Test.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef TEST_H_
#define TEST_H_

#ifdef WITH_TESTS

#include <gtest/gtest.h>
#include "TestHelper.h"

#define ASSERT_BYTE_EQ(arr1, arr2, len) {\
		for (int i = 0; i < len; ++i) {\
			ASSERT_EQ((arr1)[i], (arr2)[i]);\
		}\
}

#define ASSERT_ARRAY_EQ

static inline std::string getDataFile(const std::string& name) {
	const std::string file = __FILE__;
	const std::string folder = file.substr(0, file.find_last_of("/")) + "/_files/";
	return folder + name;
}

static inline std::string getTempFile(const std::string& name) {
	//const std::string file = __FILE__;
	//const std::string folder = file.substr(0, file.find_last_of("/")) + "/_files/";
	return "/tmp/" + name;
}


#endif



#endif /* TEST_H_ */
