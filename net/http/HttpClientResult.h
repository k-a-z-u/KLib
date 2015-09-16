#ifndef HTTPCLIENTRESULT_H
#define HTTPCLIENTRESULT_H

#include "HttpResponse.h"
#include "HttpURL.h"
#include "../../sockets/Socket.h"
#include "../../streams/BufferedInputStream.h"

namespace K {

	/** result from a HTTPClient request */
	class HttpClientResult {

	private:

		bool cleanup;
		Socket* sck;
		BufferedInputStream* bis;
		HttpResponse resp;

		/** ctor */
		friend class HttpClient;
		friend class HttpClientShared;
		HttpClientResult(const bool cleanup) : cleanup(cleanup), sck(nullptr), bis(nullptr) {;}

		/** no copy */
		HttpClientResult(const HttpClientResult& other) = delete;

	public:

		/** dtor */
		~HttpClientResult() {
			if (cleanup) {
				delete bis; bis = nullptr;
				delete sck; sck = nullptr;
			}
		}

		/** move */
		HttpClientResult(HttpClientResult&& o) {
			this->bis = o.bis; o.bis = nullptr;
			this->sck = o.sck; o.sck = nullptr;
			this->resp = o.resp;
		}

		/** get the InputStream to read from the socket */
		BufferedInputStream* getInputStream() {return bis;}

		/** get the parsed HttpResponse */
		HttpResponse& getResponse() {return resp;}

	};

}

#endif // HTTPCLIENTRESULT_H
