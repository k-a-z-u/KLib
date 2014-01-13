/*
 * TestByteArrayInputStream.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS
#include "../Test.h"
#include "../../klib/streams/ByteArrayInputStream.h"

using namespace K;

TEST(ByteArrayInputStream, readBytes) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	size_t len = strlen(data);
	ByteArrayInputStream bais((uint8_t*) data, (unsigned int) len);

	for (unsigned int i = 0; i < len; ++i) {
		ASSERT_EQ(data[i], bais.read());
	}
	ASSERT_EQ(-1, bais.read());

}

TEST(ByteArrayInputStream, readBlocks) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	size_t len = strlen(data);
	//std::cout << len << std::endl;
	ByteArrayInputStream bais((uint8_t*) data, (unsigned int) len);

	int bs = 11;
	int pos = 0;
	uint8_t buf[bs];
	int ret;

	for (int i = 0; i < 5; ++i) {
		ret = bais.read(buf, bs);
		ASSERT_EQ(bs, ret);
		ASSERT_BYTE_EQ(data+pos, buf, ret);
		pos += bs;
	}

	ret = bais.read(buf, bs);
	ASSERT_EQ(10, ret);

	ASSERT_BYTE_EQ(data+pos, buf, ret);

	ret = bais.read(buf, bs);
	ASSERT_EQ(-1, ret);

}


#endif
