#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../TestHelper.h"


#include "../../../net/http/HttpRequest.h"
#include "../../../streams/ByteArrayInputStream.h"

namespace K {

	TEST(HttpRequest, fromHeader) {

		HttpRequest r("GeT /infotext.html HttP/1.1\r\n\r\n");
		ASSERT_EQ("GET", r.getMethod());
		ASSERT_EQ("/infotext.html", r.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_1, r.getVersion());

		HttpRequest r1("POst /wiki/Katzen HTTp/1.0\r\n\r\n");
		ASSERT_EQ("POST", r1.getMethod());
		ASSERT_EQ("/wiki/Katzen", r1.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_0, r1.getVersion());

	}

	TEST(HttpRequest, asString) {

		HttpRequest r1("poST /wiki/Katzen hTTP/1.0\r\n\r\n");
		ASSERT_EQ("POST /wiki/Katzen HTTP/1.0", r1.getFirstLine());

		HttpRequest r2;
		r2.setVersion(HttpVersion::HTTP_1_1);
		r2.setMethod("PATch");
		r2.getURL().setFile("/index.html");
		ASSERT_EQ("PATCH /index.html HTTP/1.1", r2.getFirstLine());

	}

	TEST(HttpRequest, create1) {

		const std::string reqStr =
				"GET /path/file.html HTTP/1.0\r\n"
				"Host: www.example.org:88\r\n"
				"User-Agent: TestAgent/1.0\r\n"
				"\r\n"
				"data";

		ByteArrayInputStream bais((uint8_t*)reqStr.data(), reqStr.length());
		BufferedInputStream bis(&bais);
		LineInputStream lis(&bis);

		HttpRequest req(&lis);
		ASSERT_EQ("GET", req.getMethod());
		ASSERT_EQ("/path/file.html", req.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_0, req.getVersion());
		ASSERT_EQ("www.example.org", req.getURL().getHost());
		ASSERT_EQ(88, req.getURL().getPort());
		ASSERT_EQ("TestAgent/1.0", req.getHeader().get("uSeR-AgENt"));
		ASSERT_EQ("www.example.org:88", req.getHeader().get("host"));		// port-number!
		ASSERT_EQ('d', bis.peek());		// first byte of payload

	}

	TEST(HttpRequest, create2) {

		const std::string reqStr =
				"GET /path/file.html HTTP/1.0\r\n"
				"Host: www.example.org\r\n"
				"User-Agent: TestAgent/1.0\r\n"
				"\r\n"
				"payload";

		ByteArrayInputStream bais((uint8_t*)reqStr.data(), reqStr.length());
		BufferedInputStream bis(&bais);
		LineInputStream lis(&bis);

		HttpRequest req(&lis);
		ASSERT_EQ("GET", req.getMethod());
		ASSERT_EQ("/path/file.html", req.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_0, req.getVersion());
		ASSERT_EQ("www.example.org", req.getURL().getHost());
		ASSERT_EQ(80, req.getURL().getPort());
		ASSERT_EQ("TestAgent/1.0", req.getHeader().get("uSeR-AgENt"));
		ASSERT_EQ("www.example.org", req.getHeader().get("host"));
		ASSERT_EQ('p', bis.peek());		// first byte of payload

	}

	TEST(HttpRequest, create3) {

		HttpRequest req("http://www.localhost:1122/index.html", "GET", HttpVersion::HTTP_1_0);
		ASSERT_EQ("www.localhost", req.getURL().getHost());
		ASSERT_EQ(1122, req.getURL().getPort());
		ASSERT_EQ("/index.html", req.getURL().getFile());
		ASSERT_EQ("http", req.getURL().getProtocol());
		ASSERT_EQ("www.localhost:1122", req.getHeader().get("host"));		// port-numer must be present
	}

	TEST(HttpRequest, create4) {

		const std::string reqStr =
				"GET /path/file.html HTTP/1.0\r\n"
				"host: www.example.org:88\r\n"
				"\r\n"
				"something";
		const std::string reqStrB = reqStr.substr(0, reqStr.length() - 9);		// without "something"

		ByteArrayInputStream bais((uint8_t*)reqStr.data(), reqStr.length());
		BufferedInputStream bis(&bais);
		LineInputStream lis(&bis);

		HttpRequest req(&lis);
		const std::string reqStr2 = req.getRequestHeader();
		ASSERT_EQ(reqStrB, reqStr2);
		ASSERT_EQ('s', bis.peek());		// first byte of payload

	}

	TEST(HttpRequest, create5) {
		HttpRequest req("http://localhost:8888/index.html", "GET", HttpVersion::HTTP_1_0);
		ASSERT_EQ("http", req.getURL().getProtocol());
		ASSERT_EQ("localhost", req.getURL().getHost());
		ASSERT_EQ("/index.html", req.getURL().getFile());
		ASSERT_EQ(8888, req.getURL().getPort());
		ASSERT_EQ("GET", req.getMethod());
		ASSERT_EQ("localhost:8888", req.getHeader().get("host"));		// port-numer must be present
	}

	TEST(HttpRequest, params) {

		const std::string req1Str =
				"GET /the/file.html HTTP/1.0\r\n"
				"host: www.example.org:88\r\n"
				"Connection: close\r\n"
				"\r\n"
				"something";

		HttpRequest req1(req1Str);
		ASSERT_EQ(HttpConnectionMode::CLOSE, req1.getConnectionMode());


		const std::string req2Str =
				"GET /the/file.html HTTP/1.0\r\n"
				"host: www.example.org:88\r\n"
				"Connection: keep-alive\r\n"
				"\r\n"
				"something";

		HttpRequest req2(req2Str);
		ASSERT_EQ(HttpConnectionMode::KEEP_ALIVE, req2.getConnectionMode());


		const std::string req3Str =
				"GET /the/file.html HTTP/1.0\r\n"
				"host: www.example.org:88\r\n"
				"\r\n"
				"something";

		HttpRequest req3(req3Str);
		ASSERT_EQ(HttpConnectionMode::UNSPECIFIED, req3.getConnectionMode());

	}

	TEST(HttpRequest, params2) {

		HttpRequest req;

		req.setConnectionMode(HttpConnectionMode::KEEP_ALIVE);
		ASSERT_EQ(HttpConnectionMode::KEEP_ALIVE, req.getConnectionMode());

		req.setConnectionMode(HttpConnectionMode::CLOSE);
		ASSERT_EQ(HttpConnectionMode::CLOSE, req.getConnectionMode());

		req.setConnectionMode(HttpConnectionMode::UNSPECIFIED);
		ASSERT_EQ(HttpConnectionMode::UNSPECIFIED, req.getConnectionMode());

	}

}



#endif

