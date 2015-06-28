#ifndef K_NET_HTTP_HTTPSERVERLISTENER_H
#define K_NET_HTTP_HTTPSERVERLISTENER_H

//class HttpRequest;
//class HttpHeader;
//class InputStream;
//class HttpResponse;
//class BufferedInputStream;

namespace K {
	class HttpServerRequestHandler;
};

#include "HttpResponse.h"
#include "HttpRequest.h"
#include "HttpHeader.h"
#include "../../streams/InputStream.h"

namespace K {

//	struct HttpServerListenerResponse {
//		HttpResponse resp;
//		HttpHeader header;
//		InputStream* is;
//		~HttpServerListenerResponse() {
//			delete is;
//			is = nullptr;
//		}
//	};

	class HttpServerListener {

	public:

		/** dtor */
		virtual ~HttpServerListener() {;}

		/** called for every incoming request */
		virtual void onHttpRequest(HttpServerRequestHandler* handler, HttpRequest& req, InputStream& is) = 0;

	};

}

#endif // K_NET_HTTP_HTTPSERVERLISTENER_H
