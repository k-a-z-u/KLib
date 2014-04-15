/*
 * TestSockets.cpp
 *
 *  Created on: Mar 29, 2014
 *      Author: kazu
 */

#ifdef WITH_TESTS
#include "../Test.h"
#include "../TestHelper.h"
#include "../../sockets/address/NetworkAddress.h"

using namespace K;

TEST(Sockets, NetworkAddressEqual) {

	NetworkAddress adr1("localhost", 1337);
	ASSERT_EQ(1337, adr1.getPort());
	ASSERT_EQ("127.0.0.1", adr1.getHostIP());
	ASSERT_EQ("localhost", adr1.getHostName());

	NetworkAddress adr2("127.0.0.1", 1337);
	ASSERT_EQ(adr1, adr2);
	ASSERT_EQ(1337, adr2.getPort());
	ASSERT_EQ("127.0.0.1", adr2.getHostIP());
	ASSERT_EQ("localhost", adr2.getHostName());

	NetworkAddress adr3 = adr1;
	ASSERT_EQ(adr1, adr3);
	ASSERT_EQ(1337, adr3.getPort());
	ASSERT_EQ("127.0.0.1", adr3.getHostIP());
	ASSERT_EQ("localhost", adr3.getHostName());

	NetworkAddress adr4(adr1);
	ASSERT_EQ(adr1, adr4);
	ASSERT_EQ(1337, adr4.getPort());
	ASSERT_EQ("127.0.0.1", adr4.getHostIP());
	ASSERT_EQ("localhost", adr4.getHostName());

}


TEST(Sockets, NetworkAddressResolve) {

	NetworkAddress adr1("localhost", 1337);
	ASSERT_EQ("localhost", adr1.getHostName());

	NetworkAddress adr2("127.0.0.1", 1337);
	ASSERT_EQ("localhost", adr2.getHostName());

	NetworkAddress adr3("8.8.8.8", 1337);
	ASSERT_EQ("google-public-dns-a.google.com", adr3.getHostName());


}



#endif
