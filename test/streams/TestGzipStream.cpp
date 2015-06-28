/*
 * TestGzipInputStream.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS

#ifdef WITH_ZLIB


#include "../Test.h"
#include "../../streams/GzipOutputStream.h"
#include "../../streams/GzipInputStream.h"
#include "../../streams/ByteArrayOutputStream.h"
#include "../../streams/ByteArrayInOutStream.h"
#include "../../streams/ByteArrayInputStream.h"
#include "../../streams/FileOutputStream.h"
#include "../../streams/FileInputStream.h"
#include "../../streams/BufferedInputStream.h"
#include "../../streams/BufferedOutputStream.h"
#include <cmath>

using namespace K;



TEST(GzipOutputStream, writeBytes) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	unsigned int len = (unsigned int)  strlen(data);

	ByteArrayOutputStream baos;
	GzipOutputStream gos(baos);

	for (unsigned int i = 0; i < len; ++i) {
		gos.write(data[i]);
	}
	//gos.flush();
	gos.close();

}

TEST(GzipOutputStream, writeBlocks) {

	const char* data = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";
	unsigned int len = (unsigned int)  strlen(data);

	K::FileOutputStream fos("/tmp/1.gz");
	ByteArrayOutputStream baos;
	GzipOutputStream gos(fos, GzipOutputStreamHeader::MODE_GZIP);
	unsigned int bs = 11;
	unsigned int start = 0;
	bool done = false;

	while (!done) {
		unsigned int num = bs;
		if (start + num >= len) {num = len - start; done = true;}
		gos.write((uint8_t*)&data[start], num);
		start += bs;
	}

	//gos.flush();
	gos.close();

}



TEST(GzipStream, compressDecompress) {

	std::string str = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam";

	ByteArrayOutputStream baos;
	GzipOutputStream gos(baos);
	const int runs = 64;

	for (unsigned int i = 0; i < runs; ++i) {
		gos.write( (uint8_t*)str.data(), (unsigned int) str.length());
	}
	gos.close();

	ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
	GzipInputStream gis(bais);

	int i = 0;
	while(true) {
		int byte = gis.read();
		if (byte == -1) {break;}
		ASSERT_EQ( str[i%str.length()], byte );
		++i;
	}
	ASSERT_EQ(str.length() * runs, i);

}

TEST(GzipStream, compressDecompressRandom) {

	// create random data
	const unsigned int size = 256*1024;
	uint8_t rand[size];
	for (unsigned int i = 0; i < size; ++i) {
		rand[i] = (uint8_t) ::rand();
	}

	// compress
	ByteArrayOutputStream baos;
	GzipOutputStream gos(baos);
	const int bs = 1024;

	// compress per block
	for (unsigned int i = 0; i < size/bs; ++i) {
		gos.write( &rand[i*bs], bs );
	}
	gos.close();


	// decompress single bytes
	{
		ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
		GzipInputStream gis(bais);

		int i = 0;
		while(true) {
			int byte = gis.read();
			if (byte == -1) {break;}
			ASSERT_EQ( rand[i], byte );
			++i;
		}
		ASSERT_EQ(size, i);
	}

	// decompress blocks
	{
		int bs = 1337;
		uint8_t buf[bs];
		ByteArrayInputStream bais(baos.getData(), baos.getDataLength());
		GzipInputStream gis(bais);

		int i = 0;
		while(true) {
			int read = gis.read(buf, bs);
			if (read == -1) {break;}
			for (int j = 0; j < read; ++j) {
				ASSERT_EQ( rand[i], buf[j] );
				++i;
			}
		}
		ASSERT_EQ(size, i);
	}

}

TEST(GzipStream, compressDecompressFile) {

	// create random data
	const unsigned int size = 8*1024*1024;
	uint8_t* rand = (uint8_t*) malloc(size);
	for (unsigned int i = 0; i < size; ++i) {
		rand[i] = (uint8_t) ::rand();
	}

	// compress
	File ftmp = File::getTempFile();
	FileOutputStream fos(ftmp);
	GzipOutputStream gos(fos);
	int bs = 4096;

	// compress blocks
	for (unsigned int i = 0; i < size/bs; ++i) {
		gos.write(&rand[i*bs], bs);
	}
	gos.close();

	// decompress
	FileInputStream fis(ftmp);
	GzipInputStream gis(fis);
	bs = 1024;
	uint8_t buf[bs];
	int cnt = 0;

	while(true) {
		int read = gis.readFully(buf, bs);
		if (read == -1) {break;}
		for (int i = 0; i < bs; ++i) {
			ASSERT_EQ(rand[cnt], buf[i]);
			++cnt;
		}
	}
	ASSERT_EQ(cnt, size);

	// cleanup
	free(rand);
	ftmp.deleteMe();


}

TEST(GzipStream, compressDecompressFileBuffered) {

	// create random data
	const unsigned int size = 8*1024*1024;
	uint8_t* rand = (uint8_t*) malloc(size);
	for (unsigned int i = 0; i < size; ++i) {
		rand[i] = (uint8_t) ::rand();
	}

	// compress
	File ftmp = File::getTempFile();
	FileOutputStream fos(ftmp);
	BufferedOutputStream bos(fos, 4*1024);
	GzipOutputStream gos(bos);
	int bs = 4096;

	// compress blocks
	for (unsigned int i = 0; i < size/bs; ++i) {
		gos.write(&rand[i*bs], bs);
	}
	gos.close();

	// decompress
	FileInputStream fis(ftmp);
	BufferedInputStream bis(&fis, 4*1024);
	GzipInputStream gis(bis);
	bs = 1024;
	uint8_t buf[bs];
	int cnt = 0;

	while(true) {
		int read = gis.readFully(buf, bs);
		if (read == -1) {break;}
		for (int i = 0; i < bs; ++i) {
			ASSERT_EQ(rand[cnt], buf[i]);
			++cnt;
		}
	}
	ASSERT_EQ(cnt, size);

	// cleanup
	free(rand);
	ftmp.deleteMe();


}

TEST(GzipStream, compressDecompressFlush) {


	ByteArrayInOutStream baios;
	GzipOutputStream gos(baios);
	GzipInputStream gis(baios);

	for (unsigned int i = 0; i < 64; ++i) {

		// source to compress
		std::string srcStr = TestHelper::getLoremIpsum();
		uint8_t* srcData = (uint8_t*) srcStr.data();
		unsigned int len = (unsigned int) srcStr.length();

		// compress and flush
		gos.write(srcData, len);
		gos.flush();

		uint8_t buf[len];
		ASSERT_EQ(len, gis.readFully(buf, len));

		ASSERT_BYTE_EQ(srcData, buf, (int) len);

	}

}

#endif

#endif
