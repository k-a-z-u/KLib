
#ifdef WITH_TESTS
#include "../Test.h"
#include <cstdint>
#include "../../streams/dict/DictOutputStream.h"
#include "../../streams/dict/DictInputStream.h"
#include "../../streams/ByteArrayInOutStream.h"
#include "../../streams/FileInputStream.h"

using namespace K;

/** test the variable length coding */
TEST(DictStream, varLength) {

	ByteArrayInOutStream  baios;

	for (unsigned int i = 0; i < 0xFFFFF; ++i) {
		DictHelper::writeVarLength(&baios, i);
		ASSERT_EQ(i, DictHelper::readVarLength(&baios));
	}

}

TEST(DictStream, test) {


	ByteArrayInOutStream  baios;
	DictOutputStream dos(&baios);
	DictInputStream dis(&baios);

	std::string s = TestHelper::getLoremIpsum(2);//"Aus sprachwissenschaftlicher Sicht ist ein Text die sprachliche Form einer kommunikativen Handlung.";
	dos.write((uint8_t*)s.data(), (uint32_t) s.length());
	dos.close();

	std::cout << "#" << s.length() << " : " << baios.length() << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	for (unsigned int i = 0; i < (uint32_t) s.length(); ++i) {
		int r = dis.read();
		//std::cout << "#" << (char) r << std::endl;
		ASSERT_EQ(s[i], r);
	}

}

TEST(DictStream, testBinary) {


	FileInputStream fos("/tmp/box2d.tar");
	ByteArrayInOutStream  baios;
	DictOutputStream dos(&baios);
	DictInputStream dis(&baios);

	uint8_t buf[4096];
	int total = 0;
	while(true) {
		int read = fos.read(buf, 4096);
		if (read == -1) {break;}
		total += read;
		dos.write(buf, read);
	}

	dos.close();

	std::cout << total << " : " << baios.length() << std::endl;

}


#endif
