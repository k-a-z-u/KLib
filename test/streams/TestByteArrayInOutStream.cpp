/*
 * TestByteArrayInOutStream.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */







#ifdef WITH_TESTS
#include "../Test.h"
#include "../../klib/streams/ByteArrayInOutStream.h"

using namespace K;

TEST(ByteArrayInOutStream, test1) {

	ByteArrayInOutStream stream;

	ASSERT_EQ(-1, stream.read());
	ASSERT_EQ(-1, stream.read(nullptr, 1024));

	stream.write(128);
	ASSERT_EQ(128, stream.read());
	ASSERT_EQ(-1, stream.read());
	ASSERT_EQ(-1, stream.read(nullptr, 1024));

	stream.write(111);
	uint8_t dst;
	ASSERT_EQ(1, stream.read(&dst, 1024));
	ASSERT_EQ(111, dst);
	ASSERT_EQ(-1, stream.read(nullptr, 1024));

	uint8_t buf[256];
	for (unsigned int i = 0; i < 256; ++i) {buf[i] = (uint8_t)i;}
	stream.write(buf, 256);
	uint8_t buf2[256];
	ASSERT_EQ(256, stream.read(buf2, 256));
	for (unsigned int i = 0; i < 256; ++i) {ASSERT_EQ((uint8_t)i, buf2[i]);}

	stream.write(buf, 256);
	uint8_t buf3[128];
	ASSERT_EQ(128, stream.read(buf3, 128));
	for (unsigned int i = 0; i < 128; ++i) {ASSERT_EQ((uint8_t)i, buf3[i]);}
	ASSERT_EQ(128, stream.read(buf3, 128));
	for (unsigned int i = 0; i < 128; ++i) {ASSERT_EQ((uint8_t)(i+128), buf3[i]);}
	ASSERT_EQ(-1, stream.read());


}


#endif
