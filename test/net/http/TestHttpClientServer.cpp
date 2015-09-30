

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../TestHelper.h"

#define WITH_ASSERTIONS
#include "../../../net/http/HttpClient.h"
#include "../../../net/http/HttpClientShared.h"
#include "../../../net/http/HttpServer.h"
#include "../../../streams/ByteArrayInputStream.h"
#include "../../../log/LoggerStdOut.h"

#include <mutex>
#include <semaphore.h>

namespace K {

	TEST(HttpClientServer, testSyncNonShared) {

		class ServerListener : public HttpServerListener {
			virtual void onHttpRequest(HttpServerRequestHandler* handler, HttpRequest& req, InputStream& is) override {
				ASSERT_EQ("/index.html", req.getURL().getFile());
				ASSERT_EQ("localhost", req.getURL().getHost());
				ASSERT_EQ(8888, req.getURL().getPort());
				const std::string str = "response";
				ByteArrayInputStream bis((uint8_t*)str.data(), str.length());
				HttpResponse resp(HttpVersion::HTTP_1_0, 200, "OK");
				handler->respond(resp, &bis);
			}
		};

		HttpClient client;
		HttpServer server(8888);
		ServerListener listener;

		server.setListener(&listener);
		server.start();

		HttpRequest req("http://localhost:8888/index.html", "GET", HttpVersion::HTTP_1_0);
		ASSERT_EQ("localhost", req.getURL().getHost());
		ASSERT_EQ(8888, req.getURL().getPort());
		req.getHeader().add("User-Agent", "TestAgent");

		for (int i = 0; i < 8; ++i) {
			HttpClientResult res = client.requestSync(req);
			uint8_t buf[128];
			ssize_t read;
			while(true) {
				read = res.getInputStream()->read(buf, 128);
				if (read > 0) {break;}
			}
			std::string tmp((char*)buf, read);
			ASSERT_EQ("response", tmp);
		}

		server.stop();

	}

	TEST(HttpClientServer, testAsyncNonShared) {

		LoggerStdOut log;

		class ServerListener : public HttpServerListener {
			virtual void onHttpRequest(HttpServerRequestHandler* handler, HttpRequest& req, InputStream& is) override {
				ASSERT_EQ("localhost", req.getURL().getHost());
				ASSERT_EQ(8888, req.getURL().getPort());
				const std::string str = "response";
				ByteArrayInputStream bis((uint8_t*)str.data(), str.length());
				HttpResponse resp(HttpVersion::HTTP_1_1, 200, "OK");
				resp.setConnectionMode(req.getConnectionMode());
				handler->respond(resp, &bis);
			}
		};

		HttpClient client; client.setLogger(&log);
		HttpServer server(8888); server.setLogger(&log);
		ServerListener listener;

		server.setListener(&listener);
		server.start();

		class Callback : public HttpClientAsyncCallback {
		public:
			int done = 0;
			virtual void onResponse(HttpClientResult& res) override {

				// to test delays
				//std::this_thread::sleep_for (std::chrono::milliseconds(250));

				uint8_t buf[128];
				ssize_t read;
				while(true) {
					read = res.getInputStream()->read(buf, 128);
					if (read > 0) {break;}
					if (read < 0) {throw "123";}
				}
				std::string tmp((char*)buf, read);
				ASSERT_EQ("response", tmp);
				++done;
			}
		};

		HttpRequest req("http://localhost:8888/index.html", "GET", HttpVersion::HTTP_1_1);
		ASSERT_EQ("localhost", req.getURL().getHost());
		ASSERT_EQ(8888, req.getURL().getPort());
		req.getHeader().add("User-Agent", "TestAgent");
		req.setConnectionMode(HttpConnectionMode::KEEP_ALIVE);

		Callback callback;
		HttpRequest req1 = req; req1.getURL().setFile("/index.html");
		client.requestAsync(req1, &callback);
		HttpRequest req2 = req; req2.getURL().setFile("/index2.html");
		client.requestAsync(req2, &callback);
		HttpRequest req3 = req; req3.getURL().setFile("/index3.html");
		client.requestAsync(req3, &callback);

		usleep(100*1000);
		req.setConnectionMode(HttpConnectionMode::CLOSE);
		HttpRequest req4 = req; req4.getURL().setFile("/indexA.html");
		client.requestAsync(req4, &callback);
		HttpRequest req5 = req; req5.getURL().setFile("/indexB.html");
		client.requestAsync(req5, &callback);
		HttpRequest req6 = req; req6.getURL().setFile("/indexC.html");
		client.requestAsync(req6, &callback);

		while (callback.done < 6) {usleep(10000);}
		server.stop();

	}


	TEST(HttpClientServer, testAsyncShared) {

		LoggerStdOut log;

		class ServerListener : public HttpServerListener {
			virtual void onHttpRequest(HttpServerRequestHandler* handler, HttpRequest& req, InputStream& is) override {
				ASSERT_EQ("localhost", req.getURL().getHost());
				ASSERT_EQ(8888, req.getURL().getPort());
				const std::string str = "response";
				ByteArrayInputStream bis((uint8_t*)str.data(), str.length());
				HttpResponse resp(HttpVersion::HTTP_1_1, 200, "OK");
				resp.setConnectionMode(req.getConnectionMode());
				handler->respond(resp, &bis);
			}
		};

		HttpClientShared client; client.setLogger(&log);
		HttpServer server(8888); server.setLogger(&log);
		ServerListener listener;

		server.setListener(&listener);
		server.start();

		class Callback : public HttpClientAsyncCallback {
		public:
			int done = 0;
			virtual void onResponse(HttpClientResult& res) override {
				uint8_t buf[128];
				ssize_t read;
				while(true) {
					read = res.getInputStream()->read(buf, 128);
					if (read > 0) {break;}
					if (read < 0) {throw "123";}
				}
				std::string tmp((char*)buf, read);
				ASSERT_EQ("response", tmp);
				++done;
			}
		};

		HttpRequest req("http://localhost:8888/index.html", "GET", HttpVersion::HTTP_1_1);
		ASSERT_EQ("localhost", req.getURL().getHost());
		req.setConnectionMode(HttpConnectionMode::KEEP_ALIVE);

		Callback callback;

		for (int i = 0; i < 8; ++i) {
			HttpRequest req1 = req;
			req1.getURL().setFile("/index" + std::to_string(i) + ".html");
			client.requestAsync(req1, &callback);
		}

		while (callback.done < 8) {usleep(10000);}
		//client.closeAll();
		server.stop();

	}

}



#endif

