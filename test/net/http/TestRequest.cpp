

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../TestHelper.h"
#include "../../../net/http/HttpRequest.h"


namespace K {

	TEST(HttpRequest, fromHeader) {

		HttpRequest r("GeT /infotext.html HttP/1.1");
		ASSERT_EQ("GET", r.getMethod());
		ASSERT_EQ("/infotext.html", r.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_1, r.getVersion());

		HttpRequest r1("POst /wiki/Katzen HTTp/1.0");
		ASSERT_EQ("POST", r1.getMethod());
		ASSERT_EQ("/wiki/Katzen", r1.getURL().getFile());
		ASSERT_EQ(HttpVersion::HTTP_1_0, r1.getVersion());

	}

	TEST(HttpRequest, asString) {

		HttpRequest r1("poST /wiki/Katzen hTTP/1.0");
		ASSERT_EQ("POST /wiki/Katzen HTTP/1.0", r1.getAsString());

		HttpRequest r2;
		r2.setVersion(HttpVersion::HTTP_1_1);
		r2.setMethod("PATch");
		r2.getURL().setFile("/index.html");
		ASSERT_EQ("PATCH /index.html HTTP/1.1", r2.getAsString());

	}


}

#endif
