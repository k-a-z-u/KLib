
#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../TestHelper.h"


#include "../../../net/http/HttpResponse.h"
#include "../../../streams/ByteArrayInputStream.h"

namespace K {

	TEST(HttpResponse, create1) {

		HttpResponse resp(HttpVersion::HTTP_1_0, 200, "OKAY");
		resp.getHeader().add("Test", "xyz");

		std::string head = "HTTP/1.0 200 OKAY\r\ntest: xyz\r\n\r\n";
		ASSERT_EQ(head, resp.getResponseHeader());
		ASSERT_EQ("HTTP/1.0 200 OKAY", resp.getFirstLine());

	}


}



#endif

