/*
 * main.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */


#include "test/Test.h"

#include "net/http/HttpServer.h"
#include "net/http/HttpServerListener.h"
#include "net/http/HttpServerRequestHandler.h"
#include "streams/ByteArrayInputStream.h"

class MyListener : public K::HttpServerListener {

	void onHttpRequest(K::HttpServerRequestHandler* handler, K::HttpRequest& req, const K::HttpHeader& header, K::InputStream& is) {

		K::HttpResponse resp;
		resp.setCode(200);
		resp.setVersion(K::HttpVersion::HTTP_1_1);

		K::HttpHeader hResp;
		hResp.add("connection", "close");
		hResp.add("content-type", "text-html");
		hResp.add("content-length", "4");

		std::string str = "miau";

		K::ByteArrayInputStream bais( (uint8_t*) str.data(), str.length());

		handler->respond(resp, hResp, &bais);

	}

};


int main(int argc, char** argv) {

	K::HttpServer srv(8080);
	srv.setListener(new MyListener());
	srv.start();
	sleep(100);
	srv.stop();


#ifdef WITH_TESTS
	::testing::GTEST_FLAG(filter) = "*Http*";
	//::testing::GTEST_FLAG(filter) = "*Distribution*";
	//::testing::GTEST_FLAG(filter) = "*Vector*";
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif


}
