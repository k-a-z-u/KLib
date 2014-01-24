/*
 * TestByteArrayOutputStream.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS
#include "../Test.h"
#include "../../streams/ByteArrayOutputStream.h"

using namespace K;

TEST(ByteArrayOutputStream, writeBytes) {

	std::string str = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	size_t len = str.length();

	ByteArrayOutputStream baos;
	for (unsigned int i = 0; i < len; ++i) {
		baos.write(str[i]);
	}
	baos.flush();

	std::string str2((const char*) baos.getData(), baos.getDataLength());

	ASSERT_TRUE(str == str2);


}

TEST(ByteArrayOutputStream, writeBlocks) {

	std::string str = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	unsigned int len = (unsigned int) str.length();

	for (unsigned int bs = 1; bs < 128; ++bs) {

		ByteArrayOutputStream baos;
		unsigned int start = 0;
		bool done = false;

		while(!done) {
			unsigned int num = bs;
			if (start + num >= len) {num = len - start; done = true;}
			baos.write((uint8_t*)&str[start], num);
			start += bs;
		}
		baos.flush();

		std::string str2((const char*) baos.getData(), baos.getDataLength());

		ASSERT_TRUE(str == str2);

	}

}


#endif
