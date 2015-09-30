#ifndef HTTPCLIENTSHARED_H
#define HTTPCLIENTSHARED_H

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

	class HttpClientConnection {
		std::mutex mutex;
	public:
		HttpURL url;
		Socket* sck;
		BufferedInputStream* bis;
		HttpClientConnection() : sck(nullptr), bis(nullptr) {
			;
		}
		~HttpClientConnection() {
			delete bis; bis = nullptr;
			delete sck; sck = nullptr;
		}
		void lock() {mutex.lock();}
		void unlock() {mutex.unlock();}
	};


	/**
	 * this HTTP client is a little more sophisticated and will
	 * re-use already established connections instead of
	 * creating a new one for every request.
	 */
	class HttpClientShared {

	private:

		/** lock access to connection */
		std::mutex mtx;

		/** map of all established connection */
		std::unordered_map<std::string, HttpClientConnection*> hostConnections;

		/** number of retries before giving up */
		int numRetries;

		/** (debug) logging (if any) */
		Logger* log;
		static constexpr const char* logName = "HTTPc";

	public:

		/** ctor */
		HttpClientShared() : numRetries(3), log(nullptr) {
			;
		}

		/** dtor */
		~HttpClientShared() {
			closeAll();
		}

		/** ensure all open connections are closed */
		void closeAll() {
			mtx.lock();
			if (log) {log->add(logName, LogLevel::INFO, "shutdown -> cleanup");}
			for (auto it : hostConnections) { delete it.second; }
			hostConnections.clear();
			mtx.unlock();
		}

		/** set the logger to use (if any) */
		void setLogger(Logger* log) {
			this->log = log;
		}

		/** perform an asynchronous HTTP request */
		void requestAsync(HttpRequest& req, HttpClientAsyncCallback* callback) {

			// NOTE: ensure req is a COPY! otherwise, if the caller changes req while the thread is running...
			auto run = [this] (HttpRequest req, HttpClientAsyncCallback* callback) {

				if (log) {log->add(logName, LogLevel::INFO, "new (async) HTTP request: " + req.getURL());}

				// get a new/re-used connection to the host
				int numTries = this->numRetries;
				HttpClientConnection* con = getConnection(req.getURL());

				// try again (if something went wrong)
				again:;
				if (--numTries == 0) {con->unlock(); throw Exception("error during request");}

				// send request header
				if (log) {log->add(logName, LogLevel::DEBUG, "sending request: " + req.getFirstLine());}
				const std::string header = req.getRequestHeader();
				con->sck->getOutputStream()->write((uint8_t*)header.c_str(), header.length());
				con->sck->getOutputStream()->flush();

				// try to read response header
				try {

					LineInputStream lis(con->bis);
					HttpResponse resp(lis);

					// inform caller
					HttpClientResult res(false);
					res.bis = con->bis;
					res.resp = resp;
					if (log) {log->add(logName, LogLevel::DEBUG, "got response: " + res.resp.getFirstLine());}
					callback->onResponse(res);
					con->unlock();

				} catch (IOException) {

					// sending failed. reconnect and try again
					if (log) {log->add(logName, LogLevel::INFO, "HTTP connection to " + req.getURL().getHostWithPort() + " lost...");}
					reconnect(con);
					goto again;

				}

			};

			std::thread thread(run, req, callback);
			thread.detach();

		}

	private:

		/** ensure the given connection is connected to its host */
		void reconnect(HttpClientConnection* con) {

			// connect to host
			if (log) {log->add(logName, LogLevel::DEBUG, "establishing connection to: " + con->url.getHost());}
			delete con->bis; con->bis = nullptr;
			delete con->sck; con->sck = nullptr;
			con->sck = new Socket();
			con->sck->connect(NetworkAddress(con->url.getHost(), con->url.getPort()));
			con->bis = new BufferedInputStream(con->sck->getInputStream());

		}

		/** get a connection to the given host from the pool, or establish a new one */
		HttpClientConnection* getConnection(const HttpURL& url) {

			mtx.lock();

			auto it = hostConnections.find(url.getHostWithPort());

			if (it == hostConnections.end()) {

				// create a new connection to this host
				HttpClientConnection* con = new HttpClientConnection();
				con->lock();
				con->url = url;
				hostConnections[url.getHostWithPort()] = con;
				reconnect(con);
				mtx.unlock();
				return con;

			} else {

				// re-use the previous connection to this host
				it->second->lock();
				it->second->url = url;
				if (log) {log->add(logName, LogLevel::DEBUG, "reusing connection to: " + url.getHost());}
				mtx.unlock();
				return it->second;

			}

		}


	};

}

#endif // HTTPCLIENTSHARED_H
