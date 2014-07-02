
/*
 * TestTar.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: kazu
 */


#ifdef WITH_TESTS

#include "../Test.h"
#include "../../archive/tar/UnTarStream.h"
#include "../../archive/tar/TarStream.h"
#include "../../streams/OutputStream.h"
#include "../../streams/FileInputStream.h"
#include "../../streams/FileOutputStream.h"
#include "../../streams/ByteArrayInOutStream.h"

namespace K {

	TEST(TarHelper, round) {
		ASSERT_EQ(512, TarHelper::roundUp(510,512));
		ASSERT_EQ(512, TarHelper::roundUp(511,512));
		ASSERT_EQ(512, TarHelper::roundUp(512,512));
		ASSERT_EQ(1024, TarHelper::roundUp(513,512));
		ASSERT_EQ(1024, TarHelper::roundUp(514,512));
	}

	TEST(TarHelper, padding) {
		ASSERT_EQ(2, TarHelper::getPadding(510,512));
		ASSERT_EQ(1, TarHelper::getPadding(511,512));
		ASSERT_EQ(0, TarHelper::getPadding(512,512));
		ASSERT_EQ(511, TarHelper::getPadding(513,512));
	}

	//TEST(TarHeader, structSize) {
	//	ASSERT_EQ(512, sizeof(TarEntryHeader));
	//}

	TEST(TarHeader, setSize) {

		TarEntryHeader teh = TarEntryHeader::getEmptyHeader();

		teh.setSize(12345678);
		ASSERT_EQ(12345678, teh.getSize());

		teh.setSize(0);
		ASSERT_EQ(0, teh.getSize());
		//ASSERT_EQ(0, teh.getBlockedSize());

		teh.setSize(987654321);
		ASSERT_EQ(987654321, teh.getSize());

		teh.setSize(4294967295);
		ASSERT_EQ(4294967295, teh.getSize());

	}

	TEST(TarHeader, setName) {

		TarEntryHeader teh = TarEntryHeader::getEmptyHeader();

		teh.setFileName("123");
		ASSERT_EQ("123", teh.getFileName());

		// empty
		teh.setFileName("");
		ASSERT_EQ("", teh.getFileName());

		// exactly matching
		std::string str = "";
		for (unsigned int i = 0; i < 99; ++i) {str += '9';}
		teh.setFileName(str);
		ASSERT_EQ(str, teh.getFileName());

		// overlength
		std::string str2 = "";
		for (unsigned int i = 0; i < 199; ++i) {str2 += '9';}
		teh.setFileName(str2);
		ASSERT_EQ(str, teh.getFileName());

	}

	TEST(TarHeader, checkSum) {

		TarEntryHeader teh1 = TarEntryHeader::getEmptyHeader();

		// calculating the checksum multiple times must produce same output
		unsigned int c1 = teh1.calcCheckSum();
		unsigned int c2 = teh1.calcCheckSum();
		ASSERT_EQ(c1,c2);

	}

	TEST(TarHeader, timestamp) {

		TarEntryHeader teh = TarEntryHeader::getEmptyHeader();

		teh.setTimestampNow();
		uint32_t ts = teh.getTimestamp();
		ASSERT_LT(1402514139, ts);

	}


	TEST(Tar, open) {

		File folder = File(__FILE__).getParent();
		File file = File(folder, "test.tar");

		FileInputStream fis = FileInputStream(file);
		UnTarStream tar(&fis);

		unsigned int cnt = 0;
		while (tar.hasNext()) {
			tar.next();
			++cnt;
		}

		ASSERT_EQ(3, cnt);

	}



	TEST(Tar, read) {

		File folder = File(__FILE__).getParent();
		File file = File(folder, "test.tar");

		FileInputStream fis = FileInputStream(file);
		UnTarStream tar(&fis);

		unsigned int cnt = 0;
		while (tar.hasNext()) {

			UnTarEntry ute = tar.next();
			std::cout << ute.header.getTimestamp() << std::endl;

			// read to vector
			std::vector<uint8_t> vec = ute.stream.readCompletely();

			ASSERT_EQ(ute.header.getSize(), vec.size());

			if (cnt == 2) {
				std::string str((char*)vec.data(), vec.size());
				ASSERT_EQ("hallo!\n", str);
			}

			++cnt;

		}

		ASSERT_EQ(3, cnt);

	}

	TEST(Tar, write) {

		//ByteArrayInOutStream baios;
		FileOutputStream baios("xxx.tar");
		TarStream ts(&baios);

		TarEntryHeader teh1 = TarEntryHeader::getFileHeader("test1.txt", 4);
		TarEntry te = ts.addFile(teh1);
		te.stream.write((uint8_t*)"1234", 4);
		ts.close();

		//	UnTarStream uts(&baios);
		//	while (uts.hasNext()) {
		//		UnTarEntry ute = uts.next();
		//		std::cout << ute.header.getFileName() << std::endl;
		//	}

	}

	TEST(Tar, multipleClose) {

		ByteArrayInOutStream baios;
		TarStream ts(&baios);
		UnTarStream uts(&baios);

		std::string s = "asdfashjfhgajgasdhsdgfjsdfsdfsdf";
		TarEntryHeader teh1 = TarEntryHeader::getFileHeader("1.txt", (uint32_t)s.length());
		TarEntry te = ts.addFile(teh1);
		te.stream.write((uint8_t*)s.data(), (uint32_t)s.length());

		te.stream.close();
		te.stream.close();
		te.stream.close();

		TarEntryHeader teh2 = TarEntryHeader::getFileHeader("2.txt", (uint32_t)s.length());
		TarEntry te2 = ts.addFile(teh2);
		te2.stream.write((uint8_t*)s.data(), (uint32_t)s.length());

		te2.stream.close();
		te2.stream.close();

		ts.close();

		ASSERT_EQ(s.length(), uts.next().header.getSize());
		ASSERT_EQ(s.length(), uts.next().header.getSize());

	}

	TEST(Tar, readWrite) {

		ByteArrayInOutStream baios;
		TarStream ts(&baios);

		// first entry
		std::string s1 = TestHelper::getLoremIpsum();
		TarEntryHeader teh1 = TarEntryHeader::getFileHeader("1.txt", (uint32_t) s1.length());
		TarEntry te1 = ts.addFile(teh1);
		te1.stream.write((const uint8_t*) s1.data(), (uint32_t) s1.length());

		// second entry
		std::string s2 = "sandkfha skjfhsakjlfhjsalöfkjasölfjsdkflhjsflgas asdf saf asdfsadf";
		TarEntryHeader teh2 = TarEntryHeader::getFileHeader("2.txt", (uint32_t) s2.length());
		TarEntry te2 = ts.addFile(teh2);
		te2.stream.write((const uint8_t*) s2.data(), (uint32_t) s2.length());

		ts.close();

		UnTarStream uts(&baios);

		// first entry
		std::cout << "#" << s1.length() << std::endl;
		UnTarEntry ute1 = uts.next();
		ASSERT_EQ(s1.length(), ute1.header.getSize());

		// second entry
		UnTarEntry ute2 = uts.next();
		ASSERT_EQ(s2.length(), ute2.header.getSize());

	}

}

#endif
