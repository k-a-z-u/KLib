/*
 * TestLZ4OutputStream.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../../streams/LZ4OutputStream.h"
#include "../../streams/LZ4InputStream.h"
#include "../../streams/ByteArrayOutputStream.h"
#include "../../streams/ByteArrayInputStream.h"
using namespace K;

TEST(LZ4Stream, compressDecompress) {

	// demo text
	std::string lipsum = TestHelper::getLoremIpsum(16);
	uint8_t* src = (uint8_t*) lipsum.data();
	unsigned int size = (unsigned int) lipsum.size();

	// test several compression-buffer sizes
	for (unsigned int bs = 64; bs <= 32*1024; bs *=2 ) {

		ByteArrayOutputStream baos;
		LZ4OutputStream los(baos, bs);

		for (unsigned int i = 0; i < size; ++i) {
			los.write(src[i]);
		}
		los.close();

		//std::cout << "compressed using: " << bs << std::endl;

		// byte based decompression
		ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
		LZ4InputStream lis(bais);

		int cnt = 0;
		while(true) {
			int data = lis.read();
			if (data == -1) {break;}
			ASSERT_EQ(src[cnt], data);
			++cnt;
		}
		ASSERT_EQ(size, cnt);

		//std::cout << "-----------------------" << std::endl;

		// block based decompression
		ByteArrayInputStream bais2(baos.getData(), baos.getDataLength());
		LZ4InputStream lis2(bais2);

		int bsd = bs;
		uint8_t buf[bsd];
		cnt = 0;
		while(true) {
			int read = lis2.read(buf, bsd);
			if (read == -1) {break;}
			for (int j = 0; j < read; ++j) {
				ASSERT_EQ(src[cnt], buf[j]);
				++cnt;
			}
			//std::cout << cnt << std::endl;
		}
		ASSERT_EQ(size, cnt);

	}

}


#endif
