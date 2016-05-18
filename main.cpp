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

	void onHttpRequest(K::HttpServerRequestHandler* handler, K::HttpRequest& req, K::InputStream& is) {

		K::HttpResponse resp;
		resp.setCode(200);
		resp.setVersion(K::HttpVersion::HTTP_1_1);
		resp.getHeader().add("connection", "close");
		resp.getHeader().add("content-type", "text-html");
		resp.getHeader().add("content-length", "4");

		std::string str = "miau";

		K::ByteArrayInputStream bais( (uint8_t*) str.data(), str.length());

		handler->respond(resp, &bais);

	}

};


int main(int argc, char** argv) {

//	K::HttpServer srv(8080);
//	srv.setListener(new MyListener());w
//	srv.start();
//	sleep(100);
//	srv.stop();
	const double heading = 0;
	const double heading2 = 90;

	const double h1Rad = (heading / 180 * M_PI);
	const double h2Rad = (heading2 / 180 * M_PI);
	const double diffRad = std::fmod(((h1Rad-h2Rad) + M_PI), 2*M_PI) - M_PI;




#ifdef WITH_TESTS
	//::testing::GTEST_FLAG(filter) = "*LensCalibration*";
	//::testing::GTEST_FLAG(filter) = "*KDTree*";
	//::testing::GTEST_FLAG(filter) = "*OCR*";
	//::testing::GTEST_FLAG(filter) = "*Homography*";
	::testing::GTEST_FLAG(filter) = "*Outline*";
	::testing::GTEST_FLAG(filter) = "*Ellipses*";
	::testing::GTEST_FLAG(filter) = "*FundamentalMatrix*";
	//::testing::GTEST_FLAG(filter) = "*Distribution*";
	//::testing::GTEST_FLAG(filter) = "*Vector*";
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif


}
