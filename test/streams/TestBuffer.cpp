/*
 * TestBuffer.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */





#ifdef WITH_TESTS
#include "../Test.h"
#include <cstdint>
#include "../../streams/Buffer.h"

using namespace K;

TEST(Buffer, insertByte) {

	Buffer<uint8_t> buf;
	for (unsigned int i = 0; i < 1024; ++i) {
		buf.add((uint8_t) i);
	}

	ASSERT_EQ(1024, buf.getNumUsed());

	uint8_t* ptr = buf.getData();
	for (unsigned int i = 0; i < 1024; ++i) {
		ASSERT_EQ((uint8_t)i, ptr[i]);
		ASSERT_EQ((uint8_t)i, buf[i]);
	}

}

TEST(Buffer, insertShort) {

	Buffer<uint16_t> buf;
	for (unsigned int i = 0; i < 1024; ++i) {
		buf.add((uint16_t) i);
	}

	ASSERT_EQ(1024, buf.getNumUsed());

	uint16_t* ptr = buf.getData();
	for (unsigned int i = 0; i < 1024; ++i) {
		ASSERT_EQ((uint16_t)i, ptr[i]);
		ASSERT_EQ((uint16_t)i, buf[i]);
	}

}

TEST(Buffer, insertDouble) {

	Buffer<double> buf;
	for (unsigned int i = 0; i < 1024; ++i) {
		buf.add((double) i);
	}

	ASSERT_EQ(1024, buf.getNumUsed());

	double* ptr = buf.getData();
	for (unsigned int i = 0; i < 1024; ++i) {
		ASSERT_EQ((double)i, ptr[i]);
		ASSERT_EQ((double)i, buf[i]);
	}

}

TEST(Buffer, remove) {

	Buffer<int> buf;

	buf.resize(16);
	ASSERT_EQ(0, buf.getNumUsed());
	ASSERT_EQ(16*sizeof(int), buf.getMemoryConsumption());

	for (unsigned int i = 0; i < 16; ++i) {
		buf[i] = i;
	}

	for (unsigned int i = 0; i < 16; ++i) {
		ASSERT_EQ(i, buf[i]);
	}

	buf.setNumUsed(16);
	ASSERT_EQ(16, buf.getNumUsed());

	buf.remove(4);
	ASSERT_EQ(16-4, buf.getNumUsed());

	for (unsigned int i = 0; i < 16-4; ++i) {
		ASSERT_EQ(i+4, buf[i]);
	}

}

#endif
