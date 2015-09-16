#ifndef K_NET_HTTP_HTTPCLIENT_H
#define K_NET_HTTP_HTTPCLIENT_H

#include <thread>
#include <unordered_map>
#include <mutex>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpHeader.h"
#include "HttpClientResult.h"
#include "HttpClientAsyncCallback.h"

#include "../../log/Logger.h"
#include "../../sockets/Socket.h"

#include "../../streams/InputStream.h"
#include "../../streams/OutputStream.h"
#include "../../streams/BufferedOutputStream.h"
#include "../../streams/BufferedInputStream.h"

#include "../../sockets/SocketOutputStream.h"
#include "../../sockets/SocketInputStream.h"

namespace K {

	/**
	 * a very simple HTTP client.
	 * for each request a new connection is established
	 */
	class HttpClient {

		struct Connection {

			Socket* sck;
			BufferedInputStream* bis;

			Connection() {
				sck = new Socket();
				bis = new BufferedInputStream(sck->getInputStream());
			}

			~Connection() {
				delete sck; sck = nullptr;
				delete bis; bis = nullptr;
			}

		};

	private:

		/** (debug) logging, if any */
		Logger* log;
		static constexpr const char* logName = "HTTPc";

	public:

		/** ctor */
		HttpClient() : log(nullptr) {
			;
		}

		/** dtor */
		~HttpClient() {
			if (log) {log->add(logName, LogLevel::INFO, "shutdown -> cleanup");}
		}

		/** set the logger to use (if any) */
		void setLogger(Logger* log) {
			this->log = log;
		}

		/** perform a synchronous HTTP-GET request on the given URL */
		HttpClientResult requestSync(const std::string& url) {
			HttpRequest req(url, "GET", HttpVersion::HTTP_1_0);
			return requestSync(req);
		}

		/** perform a synchronous HTTP request */
		HttpClientResult requestSync(HttpRequest& req) {

			if (log) {log->add(logName, LogLevel::INFO, "new (sync) HTTP request: " + req.getURL());}

			// get a connection to the host
			HttpClientResult res = connect(req.getURL());

			// send request header
			sendHeader(res, req);

			// try to read response header
			LineInputStream lis(res.bis);
			res.resp = HttpResponse(lis);

			// move result to the caller
			if (log) {log->add(logName, LogLevel::DEBUG, "got response: " + res.resp.getFirstLine());}

			return res;

		}

		/** perform an asynchronous HTTP request */
		void requestAsync(HttpRequest& req, HttpClientAsyncCallback* callback) {

			// NOTE: ensure req is a COPY! otherwise, if the caller changes req while the thread is running...
			auto run = [this] (HttpRequest req, HttpClientAsyncCallback* callback) {

				if (log) {log->add(logName, LogLevel::INFO, "new (async) HTTP request: " + req.getURL());}

				// get a connection to the host
				HttpClientResult res = connect(req.getURL());

				// send request header
				sendHeader(res, req);

				// try to read response header
				LineInputStream lis(res.bis);
				res.resp = HttpResponse(lis);

				// inform caller
				if (log) {log->add(logName, LogLevel::DEBUG, "got response: " + res.resp.getFirstLine());}
				callback->onResponse(res);

			};

			// run async
			std::thread thread(run, req, callback);
			thread.detach();

		}

	private:

		/** send the HTTP-header to the host */
		void sendHeader(HttpClientResult& con, HttpRequest& req) const {
			if (log) {log->add(logName, LogLevel::DEBUG, "sending request: " + req.getFirstLine());}
			const std::string header = req.getRequestHeader();
			con.sck->getOutputStream()->write((uint8_t*)header.c_str(), header.length());
			con.sck->getOutputStream()->flush();
		}

		/** connect to the host behind the given URL */
		HttpClientResult connect(const HttpURL& url) const {
			if (log) {log->add(logName, LogLevel::DEBUG, "establishing connection to: " + url.getHostWithPort());}
			HttpClientResult res(true);
			res.sck = new Socket();
			res.sck->connect(NetworkAddress(url.getHost(), url.getPort()));
			res.bis = new BufferedInputStream(res.sck->getInputStream());
			return res;
		}

	};

}

#endif // K_NET_HTTP_HTTPCLIENT_H
