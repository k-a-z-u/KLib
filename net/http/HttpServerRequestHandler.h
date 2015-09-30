#ifndef K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H
#define K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H

#include <atomic>

#include "../../streams/InputStream.h"
#include "../../streams/OutputStream.h"
#include "../../streams/BufferedInputStream.h"
#include "../../streams/LineInputStream.h"

#include "../../sockets/SocketOutputStream.h"
#include "../../sockets/SocketInputStream.h"

#include "../../log/Logger.h"

#include "HttpServerListener.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpHeader.h"

namespace K {

	/**
	 * handle (parse) a single HTTP request
	 */
	class HttpServerRequestHandler {

	private:

		/** the socket behind the incoming request */
		Socket* sck;

		/** the listener to inform after parsing the request */
		HttpServerListener* listener;

		SocketInputStream* is;
		SocketOutputStream* os;

		BufferedInputStream* bis;
		LineInputStream* lis;

		/** is this connection currently active or closed? */
		std::atomic<bool> closed;

		/** the logger to use (if any) */
		Logger* log;
		static constexpr const char* logName = "HTTPh";
		static constexpr int BUF_SIZE = 16*1024;

	public:

		/**
		 * ctor.
		 * the socket will be deleted on destruction!
		 */
		HttpServerRequestHandler(Socket* sck, HttpServerListener* listener, Logger* log) :
			sck(sck), listener(listener), is(sck->getInputStream()), os(sck->getOutputStream()), log(log) {
			bis = new BufferedInputStream(is);
			lis = new LineInputStream(bis);
		}

		/** dtor */
		~HttpServerRequestHandler() {
			delete lis; lis = nullptr;
			delete bis; bis = nullptr;
			delete sck;	sck = nullptr;
		}

		/**
		 * blocking loop that:
		 * reads the next incoming HTTP-request
		 * informs a listener, that will create a response
		 */
		void handle() {

			closed = false;

			while (!closed) {

				try {

					// parse the (next) incoming http request
					HttpRequest req(lis);
					if (log) {log->add(logName, LogLevel::DEBUG, "got request header: " + req.getFirstLine());}

					// call listener to send a response
					listener->onHttpRequest(this, req, *bis);

				} catch (IOException) {
					if (log) {log->add(logName, LogLevel::DEBUG, "got IOException. Client closed connection?");}
					close();
					break;
				}

			}

		}

		/** cancel the handling process */
		void cancel() {
			close();
		}

		/**
		 * send the given response back to the other party.
		 * if an input-stream is given, its contents will be sent as payload.
		 * blocks until everything is sent.
		 */
		void respond(HttpResponse& resp, InputStream* is) {

			if (log) {log->add(logName, LogLevel::DEBUG, "responding with: " + resp.getFirstLine());}

			// send response header
			const std::string& header = resp.getResponseHeader();
			os->write((uint8_t*)header.data(), header.length());
			os->flush();

			// send response payload (if any)
			if (is) {
				if (log) {log->add(logName, LogLevel::DEBUG, "sending payload");}
				uint8_t buf[BUF_SIZE];
				while(true) {
					const ssize_t read = is->read(buf, BUF_SIZE);
					if (read == InputStream::ERR_FAILED) {break;}
					if (read > 0) {	os->write(buf, read); }
				}
				is->close();
				os->flush();
			}

			// close the connection?
			if (resp.getVersion() == HttpVersion::HTTP_1_0) {
				if (log) {log->add(logName, LogLevel::DEBUG, "HTTP/1.0 -> closing connection");}
				close();
			} else if (resp.getConnectionMode() == HttpConnectionMode::CLOSE) {
				if (log) {log->add(logName, LogLevel::DEBUG, "connection: close -> closing connection");}
				close();
			} else {
				if (log) {log->add(logName, LogLevel::DEBUG, "connection: keep-alive");}
			}

		}	

	private:

		/** close the connection and mark myself as closed */
		void close() {
			if (!closed) {
				is->close();
				os->close();
				sck->close();
				closed = true;
			}
		}

	};
}

#endif // K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H
