#ifndef K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H
#define K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H

#include "../../streams/InputStream.h"
#include "../../streams/OutputStream.h"
#include "../../streams/BufferedInputStream.h"
#include "../../streams/LineInputStream.h"

#include "../../sockets/SocketOutputStream.h"
#include "../../sockets/SocketInputStream.h"

#include "HttpServerListener.h"
#include "HttpRequest.h"
#include "HttpHeader.h"

namespace K {

	/**
	 * handle (parse) a single HTTP request
	 */
	class HttpServerRequestHandler {


	public:

		/**
		 * ctor.
		 * the socket will be deleted on destruction!
		 */
		HttpServerRequestHandler(Socket* sck, HttpServerListener* listener) :
			sck(sck), listener(listener), is(sck->getInputStream()), os(sck->getOutputStream()) {

		}

		/** dtor */
		~HttpServerRequestHandler() {
			delete sck;
			sck = nullptr;
		}

		/** magic */
		void handle() {

			// create streams
			BufferedInputStream bis(is);
			LineInputStream lis(&bis);
			HttpRequest req(lis);

			// call listener
			listener->onHttpRequest(this, req, bis);

		}

		void respond(HttpResponse& resp, HttpHeader& header, InputStream* is) {

			static uint8_t nl[] = {'\r', '\n'};

			// send response header
			const std::string s1 = resp.getAsString();
			os->write( (uint8_t*) s1.data(), s1.length());
			const std::string s2 = header.getAsString();
			os->write( (uint8_t*) s2.data(), s2.length());
			os->write( nl, 2);
			os->flush();

			// send response payload (if any)
			if (is) {

				while(true) {
					uint8_t buf[4096];
					const int read = is->read(buf, 4096);
					if (read == -1) {break;}
					os->write(buf, read);
				}
				os->flush();

			}

		}

	private:

		/** the socket behind the incoming request */
		Socket* sck;

		/** the listener to inform after parsing the request */
		HttpServerListener* listener;

		SocketInputStream* is;
		SocketOutputStream* os;


	};
}

#endif // K_NET_HTTP_HTTPSERVERREQUESTHANDLER_H
