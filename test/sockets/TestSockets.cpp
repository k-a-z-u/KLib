/*
 * TestSockets.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../TestHelper.h"
#include "../../sockets/ServerSocket.h"
#include "../../sockets/Socket.h"

using namespace K;

TEST(Sockets, data) {

	ServerSocket ssck;
	ssck.bind(1337);

	Socket sck1;
	sck1.connect("127.0.0.1", 1337);

	Socket* sck2 = ssck.accept();

	std::string lipsum = TestHelper::getLoremIpsum();
	uint8_t* data = (uint8_t*) lipsum.data();
	unsigned int len = (unsigned int) lipsum.size();

	SocketInputStream s1in = sck1.getInputStream();
	SocketOutputStream s2out = sck2->getOutputStream();

	s2out.write(128);
	ASSERT_EQ(128, s1in.read());

	s2out.write(data, len);
	for (unsigned int i = 0; i < len; ++i) {
		ASSERT_EQ(data[i], s1in.read());
	}

	s1in.close();
	ASSERT_THROW(s1in.read(), SocketException);
	s2out.write(128);
	ASSERT_THROW(s2out.write(128), SocketException);

	//ASSERT_EQ(-1, s1in.read());


	delete sck2;

}


#endif
