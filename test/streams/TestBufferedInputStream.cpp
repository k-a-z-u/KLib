/*
 * TestBufferedInputStream.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../../streams/ByteArrayInputStream.h"
#include "../../streams/BufferedInputStream.h"

using namespace K;

TEST(BufferedInputStream, readBytes) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	unsigned int len = (unsigned int)  strlen(data);

	ByteArrayInputStream bais((uint8_t*) data, len);
	BufferedInputStream bis(&bais,1);


	for (unsigned int i = 0; i < len; ++i) {
		ASSERT_EQ(data[i], bis.read());
	}

	ASSERT_EQ(-1, bis.read());

}

TEST(BufferedInputStream, readBlocks) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	unsigned int len = (unsigned int)  strlen(data);

	for (unsigned int size = 1; size < 127; ++size) {

		ByteArrayInputStream bais((uint8_t*) data, len);
		BufferedInputStream bis(&bais, size);

		unsigned int bs = 11;
		uint8_t buf[bs];
		int pos = 0;
		int read;

		for (unsigned int i = 0; i < 5; ++i) {
			read = bis.read(buf, bs);
			ASSERT_EQ(bs, read);
			ASSERT_BYTE_EQ(data+pos, buf, read);
			pos += bs;
		}

		read = bis.read(buf, bs);
		ASSERT_EQ(10, read);
		ASSERT_BYTE_EQ(data+pos, buf, 10);

		read = bis.read(buf, bs);
		ASSERT_EQ(-1, read);

	}

}

#endif
