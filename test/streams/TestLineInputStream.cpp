/*
 * TestLineInputStream.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS

#include "../Test.h"

#include "../../streams/LineInputStream.h"
#include "../../streams/BufferedInputStream.h"
#include "../../streams/ByteArrayInputStream.h"

using namespace K;

TEST(LineInputStream, readLines) {

	std::string s1 = "Lorem ipsum dolor sit amet.";
	std::string s2 = "consetetur sadipscing elitr, sed diam nonumy eirmod.";
	std::string s3 = "tempor invidunt ut labore et dolore magna aliquyam erat.";
	std::string s4 = "sed diam voluptua.";
	std::string s5 = "At vero eos et accusam et justo duo dolores.";
	std::string s6 = "et ea rebum. Stet clita kasd gubergren, no sea takimata.";
	std::string s7 = "sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolo";

	std::string str = s1 + "\n" + s2 + "\r\n\n" + s3 + "\r" + s4 + "\r\n\r\n" + s5 + "\n\n" + s6 + "\n" + s7;
	const char* data = str.data();

	unsigned int len = (unsigned int) str.length();

	ByteArrayInputStream bais((uint8_t*) data, len);
	BufferedInputStream bis(&bais);
	LineInputStream lis(&bis);


	ASSERT_TRUE(s1 == lis.readLine());
	ASSERT_TRUE(s2 == lis.readLine());
	ASSERT_TRUE(lis.readLine().empty());
	ASSERT_TRUE(s3 == lis.readLine());
	ASSERT_TRUE(s4 == lis.readLine());
	ASSERT_TRUE(lis.readLine().empty());
	ASSERT_TRUE(s5 == lis.readLine());
	ASSERT_TRUE(lis.readLine().empty());
	ASSERT_TRUE(s6 == lis.readLine());
	ASSERT_TRUE(s7 == lis.readLine());

}

#endif
