
#ifdef WITH_TESTS
#include "../Test.h"
#include <cstdint>
#include "../../streams/window/WindowOutputStream.h"
#include "../../streams/window/WindowInputStream.h"
#include "../../streams/ByteArrayInOutStream.h"
#include "../../streams/FileInputStream.h"

using namespace K;

///** test the variable length coding */
//TEST(WindowStream, test) {

//	ByteArrayInOutStream  baios;
//	WindowOutputStream wos(&baios);

//	for (unsigned int i = 0; i < 0xFF; ++i) {
//		wos.write(i);
//	}

//}

///** test the variable length coding */
//TEST(WindowStream, test2) {

//	WindowBuffer buf;

//	buf.add("hallo wie wie wie wie geht es dir");

//	WindowMatch wm = buf.getLongestMatch("wie geht");
//	std::cout << wm.len << ":" << wm.idx << std::endl;

//}

///** test the variable length coding */
//TEST(WindowStream, test3) {

//	ByteArrayInOutStream  baios;
//	WindowOutputStream wos(&baios);

//	std::string s = "man findet keine freunde mit salat";
//	wos.write((uint8_t*)s.data(), s.length());

//}



/** test the variable length coding */
TEST(WindowStream, test4) {

	std::cout << "-------------------------------------" << std::endl;

	ByteArrayInOutStream  baios;
	WindowOutputStream wos(&baios);
	WindowInputStream wis(&baios);

	//std::string s = "man findet keine freunde mit salat";
	std::string s = TestHelper::getLoremIpsum(5);
	wos.write((uint8_t*)s.data(), s.length());
	wos.close();

	std::cout << s.length() << " -> " << baios.length() << std::endl;

	std::string s2 = "";


	while(true) {
		int i = wis.read();
		if (i == -1) {break;}
		s2 += (char) i;
		//std::cout << "#" << i << std::endl;
	}
	wis.close();;

	ASSERT_EQ(s, s2);


}

/** test the variable length coding */

TEST(WindowStream, test5) {

	FileInputStream fis("/tmp/box2d.tar");
	ByteArrayInOutStream  baios;
	WindowOutputStream wos(&baios);
	WindowInputStream wis(&baios);

	uint8_t buf[4096];
	uint32_t total = 0;
	while(true) {
		int read = fis.read(buf, 4096);
		if (read == -1) {break;}
		wos.write(buf, read);
		total += read;
		float percent = (float) baios.length() / (float) total;
		std::cout << total << " -> " << baios.length() << " : " << percent << std::endl;
	}
	wos.close();

	std::cout << total << " -> " << baios.length() << std::endl;

//	std::string s2 = "";


//	while(true) {
//		int i = wis.read();
//		if (i == -1) {break;}
//		s2 += (char) i;
//		//std::cout << "#" << i << std::endl;
//	}
//	wis.close();;

//	ASSERT_EQ(s, s2);

}


#endif
