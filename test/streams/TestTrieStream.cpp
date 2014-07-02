
#ifdef WITH_TESTS
#include "../Test.h"
#include <cstdint>
#include "../../streams/window/TrieStream.h"
#include "../../streams/window/TrieInputStream.h"
#include "../../streams/ByteArrayInOutStream.h"
#include "../../streams/BufferedInputStream.h"
#include "../../streams/FileInputStream.h"

using namespace K;

void trieStreamTestWithString(const std::string& s) {

	ByteArrayInOutStream  baios;
	TrieStream ts(&baios);
	TrieInputStream tis(&baios);

	// write the complete pattern
	ts.write((uint8_t*)s.data(), (uint32_t)s.length());
	ts.close();

	// read back
	std::string s2;
	while(true) {
		int i = tis.read();
		if (i == -1) {break;}
		s2 += (char) i;
	}

	// check
	ASSERT_EQ(s.length(), s2.length());
	ASSERT_EQ(s, s2);

}

TEST(TrieStream, missingPattern) {
	trieStreamTestWithString("aaaa");
}

TEST(TrieStream, txt1) {
	trieStreamTestWithString(TestHelper::getBlindText1());
}

TEST(TrieStream, txt2) {
	trieStreamTestWithString(TestHelper::getLoremIpsum());
}

TEST(TrieStream, txt3) {
	trieStreamTestWithString(TestHelper::getLoremIpsum(10));
}


///** test the variable length coding */
//TEST(TrieStream, test1) {

//	ByteArrayInOutStream  baios;
//	TrieStream ts(&baios);
//	TrieInputStream tis(&baios);
//	//std::string s = "man findet keine freunde mit salat und das ist selbstverstaendlich auch sehr sehr gut so";
//	//std::string s = TestHelper::getBlindText1();
//	std::string s = TestHelper::getLoremIpsum(2);

//	ts.write((uint8_t*)s.data(), s.length());
//	ts.close();

//	std::cout << s.length() << " : " << baios.length() << std::endl;

//		std::string s2 = "";
//		while(true) {
//			//sleep(1);
//			int i = tis.read();
//			if (i == -1) {break;}
//			//std::cout << "*" << (char) i << std::endl;
//			s2 += (char) i;
//			//std::cout << s2.length() << std::endl;
//		}

//		ASSERT_EQ(s, s2);



//}

TEST(TrieStream, test2) {

	FileInputStream fis("/tmp/box2d.tar");
	FileInputStream fis2("/tmp/box2d.tar");
	ByteArrayInOutStream  baios;
	TrieStream ts(&baios);
	//BufferedInputStream bis(baios);
	TrieInputStream tis(&baios);

	uint8_t buf[4096];
	uint32_t total = 0;
	while(true) {
		int read = fis.read(buf, 4096);
		if (read == -1) {break;}
		ts.write(buf, read);
		total += read;
		float percent = (float) baios.length() / (float) total;
		std::cout << total << " -> " << baios.length() << " : " << percent << std::endl;
	}
	ts.close();

	std::cout << total << " -> " << baios.length() << std::endl;

	// read and check
	int cnt = 0;
	while(true) {
		if ( ((++cnt) % 512) == 0 ) {std::cout << cnt << std::endl;}
		//std::cout << ++cnt << std::endl;
		int i1 = tis.read();
		int i2 = fis2.read();
		ASSERT_EQ(i1, i2);
		if (i2 == -1) {break;}
	}

}

#endif
