

#ifdef WITH_TESTS


#include "../../Test.h"
#include "../../TestHelper.h"

#define WITH_ASSERTIONS
#include "../../../net/http/HttpURL.h"


namespace K {

	TEST(HttpUrl, create1) {

		HttpURL url("http://www.example.org:8080/123/index.html");
		ASSERT_EQ("www.example.org", url.getHost());
		ASSERT_EQ("www.example.org:8080", url.getHostWithPort());
		ASSERT_EQ(8080, url.getPort());
		ASSERT_EQ("http", url.getProtocol());
		ASSERT_EQ("/123/index.html", url.getFile());
		ASSERT_EQ("http://www.example.org:8080/123/index.html", url.getURL());

	}

	TEST(HttpUrl, create2) {

		HttpURL url("https://www.example.org:80/123/index.html");
		ASSERT_EQ("www.example.org", url.getHost());
		ASSERT_EQ("www.example.org", url.getHostWithPort());
		ASSERT_EQ(80, url.getPort());
		ASSERT_EQ("https", url.getProtocol());
		ASSERT_EQ("/123/index.html", url.getFile());
		ASSERT_EQ("https://www.example.org/123/index.html", url.getURL());

	}

	TEST(HttpUrl, create3) {

		HttpURL url;
		url.setProtocol("ftp");
		url.setPort(1234);
		url.setFile("/website/index.html");
		url.setHost("example.com");
		ASSERT_EQ("ftp://example.com:1234/website/index.html", url.getURL());

	}

	TEST(HttpUrl, createMissing) {

		HttpURL url;
		ASSERT_ANY_THROW(url.getURL());

	}

}



#endif

