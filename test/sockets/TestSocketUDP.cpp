
/*
 * TestSockets.cpp
 *
 *  Created on: Mar 27, 2014
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "../Test.h"
#include "../TestHelper.h"
#include "../../sockets/SocketUDP.h"
#include "../../sockets/datagram/WrapperDatagram.h"

using namespace K;

TEST(SocketUDP, FaultyDestination) {

	SocketUDP sck;
	sck.bind(1337);

	NetworkAddress adr1;
	NetworkAddress adr2(1338);
	NetworkAddress adr3("127.0.0.1", 1338);

	uint8_t data[] = {1};

	ASSERT_THROW( sck.sendDatagram(data, 1, adr1), SocketException );
	ASSERT_THROW( sck.sendDatagram(data, 1, adr2), SocketException );
	ASSERT_NO_THROW( sck.sendDatagram(data, 1, adr3) );

}

TEST(SocketUDP, FaultySize) {

	SocketUDP sck;
	sck.bind(1337);

	NetworkAddress adr1("127.0.0.1", 1338);

	uint8_t* data = nullptr;

	ASSERT_THROW( sck.sendDatagram(data, 128*1024, adr1), SocketException );
	ASSERT_NO_THROW( sck.sendDatagram(data, 0, adr1) );

}

TEST(SocketUDP, DefaultDatagram) {

	SocketUDP sck1;
	sck1.bind(1337);
	NetworkAddress addr1("127.0.0.1", 1337);

	SocketUDP sck2;
	sck2.bind(1338);
	NetworkAddress addr2("127.0.0.1", 1338);

	std::vector<uint8_t> data = {1,2,3};
	sck1.sendDatagram(data, addr2);

	DefaultDatagram dd = sck2.receiveDatagram();
	ASSERT_EQ(data.size(), dd.getLength());
	for (unsigned int i = 0; i < data.size(); ++i) {
		ASSERT_EQ(data[i], dd.getData()[i]);
	}

}


TEST(SocketUDP, WrapperDatagram) {

	SocketUDP sck1;
	sck1.bind(1337);
	NetworkAddress addr1("127.0.0.1", 1337);

	SocketUDP sck2;
	sck2.bind(1338);
	NetworkAddress addr2("127.0.0.1", 1338);

	// send 5 bytes of data
	uint8_t data[] = {5,4,3,2,1};
	WrapperDatagram wd(data, 5, addr1);
	sck2.sendDatagram(wd);

	DefaultDatagram dd;
	sck1.receiveDatagram(dd);

	ASSERT_EQ(5, dd.getLength());
	for (unsigned int i = 0; i < 5; ++i) {
		ASSERT_EQ(data[i], dd.getData()[i]);
	}

	// answer with zero bytes of data
	sck1.sendDatagram(nullptr, 0, addr2);
	sck2.receiveDatagram(dd);
	ASSERT_EQ(0, dd.getLength());

}

/** send random data of random size in both directions */
TEST(SocketUDP, RandomData) {

	SocketUDP sck1;
	sck1.bind(1337);
	NetworkAddress addr1("127.0.0.1", 1337);

	SocketUDP sck2;
	sck2.bind(1338);
	NetworkAddress addr2("127.0.0.1", 1338);

	const int cnt = 1024;
	DefaultDatagram ddOut;
	DefaultDatagram ddIn;

	for (unsigned int i = 0; i < cnt; ++i) {

		// build random data
		unsigned int rndLen = rand() % 65536;
		ddOut.ensureSpace(rndLen);
		ddOut.setLength(rndLen);
		for (unsigned int j = 0; j < rndLen; ++j) {
			ddOut.getDataWriteable()[j] = (uint8_t) ( rand() & 256 );
		}

		// send
		ddOut.setAddress(addr2);
		sck1.sendDatagram(ddOut);

		// receive
		sck2.receiveDatagram(ddIn);

		// ensure sender address is correct
		ASSERT_EQ(ddIn.getAddress(), addr1);
		ASSERT_EQ(ddIn.getAddress().getHostIP(), addr1.getHostIP());
		ASSERT_EQ(ddIn.getAddress().getPort(), addr1.getPort());

		// ensure correct length and data
		ASSERT_EQ(rndLen, ddIn.getLength());
		ASSERT_EQ( 0, memcmp(ddOut.getData(), ddIn.getData(), rndLen) );

	}



}



#endif
