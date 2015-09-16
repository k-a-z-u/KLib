#ifndef K_NET_HTTP_HTTPCLIENTASYNCCALLBACK_H
#define K_NET_HTTP_HTTPCLIENTASYNCCALLBACK_H

#include "HttpClientResult.h"

namespace K {


	/** callbacl for async HTTP requests */
	class HttpClientAsyncCallback {

	public:

		virtual ~HttpClientAsyncCallback() {;}

		/** received a response to an async HTTP request */
		virtual void onResponse(HttpClientResult& res) = 0;

	};

}

#endif // K_NET_HTTP_HTTPCLIENTASYNCCALLBACK_H
