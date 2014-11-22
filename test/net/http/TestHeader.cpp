

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../TestHelper.h"
#include "../../../net/http/HttpHeader.h"


namespace K {

	TEST(HttpHeader, create) {

		HttpHeader h;
		h.add("Connection", "close");
		h.add("Content-Length", "1337");
		h.add("Server", "TestServer");

		std::string sh = "server: TestServer\r\ncontent-length: 1337\r\nconnection: close\r\n";

		ASSERT_EQ(sh, h.getAsString());

	}

	TEST(Httpheader, contains) {

		HttpHeader h;
		h.add("Connection", "close");
		h.add("Content-Length", "1337");
		h.add("Server", "TestServer");

		ASSERT_TRUE(h.contains("connection"));
		ASSERT_TRUE(h.contains("conNectioN"));
		ASSERT_TRUE(h.contains("Content-Length"));
		ASSERT_TRUE(h.contains("content-length"));
		ASSERT_TRUE(h.contains("SERVER"));
		ASSERT_TRUE(h.contains("server"));

		ASSERT_FALSE(h.contains("other"));

	}

	TEST(Httpheader, get) {

		HttpHeader h;
		h.add("Connection", "close");
		h.add("Content-Length", "1337");
		h.add("Server", "TestServer");

		ASSERT_EQ("close", h.get("connection"));
		ASSERT_EQ("1337", h.get("Content-Length"));
		ASSERT_EQ("TestServer", h.get("SERVER"));

	}

	TEST(Httpheader, addLine) {

		HttpHeader h;
		h.addLine("Connection: close\r");
		h.addLine("Content-Length: 1337\r\n");
		h.addLine("Content-Type: text/html\n");
		h.addLine("Server: TestServer");

		ASSERT_EQ("close", h.get("connection"));
		ASSERT_EQ("1337", h.get("Content-Length"));
		ASSERT_EQ("TestServer", h.get("SERVER"));
		ASSERT_EQ("text/html", h.get("content-TYPE"));

	}

}



#endif

