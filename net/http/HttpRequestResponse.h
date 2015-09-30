#ifndef K_NET_HTTP_HTTPREQUESTRESPONSE_H
#define K_NET_HTTP_HTTPREQUESTRESPONSE_H

#include "HttpHeader.h"
#include "HttpEnums.h"

namespace K {

	/**
	 * base-class for both:
	 * - HttpRequests
	 * - HttpResponses
	 */
	class HttpRequestResponse {

	protected:

		/** the response header */
		HttpHeader header;

	public:

		/** get the contained HTTP-header */
		HttpHeader& getHeader() {return header;}

		/** get the header's HTTP connection mode (keep-alive, close) */
		HttpConnectionMode getConnectionMode() {
			if (!header.contains("connection")) {return HttpConnectionMode::UNSPECIFIED;}
			const std::string mode = HttpHelper::toLower(header.get("connection"));
			if ("close" == mode)		{return HttpConnectionMode::CLOSE;}
			if ("keep-alive" == mode)	{return HttpConnectionMode::KEEP_ALIVE;}
			return HttpConnectionMode::UNSPECIFIED;
		}

		/** set the header's HTTP connection mode (keep-alive, close) */
		void setConnectionMode(const HttpConnectionMode mode) {
			switch (mode) {
				case HttpConnectionMode::UNSPECIFIED: header.remove("connection"); break;
				case HttpConnectionMode::CLOSE: header.add("connection", "close"); break;
				case HttpConnectionMode::KEEP_ALIVE: header.add("connection", "keep-alive"); break;
			}
		}

	};

}

#endif // K_NET_HTTP_HTTPREQUESTRESPONSE_H
