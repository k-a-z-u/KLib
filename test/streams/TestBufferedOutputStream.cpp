/*
 * TestBufferedOutputStream.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../../streams/ByteArrayOutputStream.h"
#include "../../streams/ByteArrayInputStream.h"
#include "../../streams/BufferedOutputStream.h"
#include "../../streams/BufferedInputStream.h"

using namespace K;

TEST(BufferedOutputStream, writeBytes) {

	unsigned int size = 1024;
	uint8_t* rand = TestHelper::getRandom(size);

	ByteArrayOutputStream baos;
	BufferedOutputStream bos(baos);

	// write all bytes
	for (unsigned int i = 0; i < size; ++i) {
		bos.write(rand[i]);
	}
	bos.close();

	ASSERT_EQ(size, baos.getDataLength());

	ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
	BufferedInputStream bis(bais);

	int cnt = 0;
	while(true) {
		int data = bis.read();
		if (data == -1) {break;}
		ASSERT_EQ(rand[cnt], data);
		++cnt;
	}
	ASSERT_EQ(size, cnt);

	// cleanup
	free(rand);

}

TEST(BufferedOutputStream, writeBlock) {

	unsigned int size = 1024*1024;
	uint8_t* rand = TestHelper::getRandom(size);

	int bs = 4096;
	ByteArrayOutputStream baos;
	BufferedOutputStream bos(baos);

	// write all bytes
	for (unsigned int i = 0; i < size/bs; ++i) {
		bos.write(&rand[i*bs], bs);
	}
	bos.close();

	ASSERT_EQ(size, baos.getDataLength());

	ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
	BufferedInputStream bis(bais);

	int cnt = 0;
	while(true) {
		int data = bis.read();
		if (data == -1) {break;}
		ASSERT_EQ(rand[cnt], data);
		++cnt;
	}
	ASSERT_EQ(size, cnt);

	// cleanup
	free(rand);

}

#endif
