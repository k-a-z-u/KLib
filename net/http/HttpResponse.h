#ifndef K_NET_HTTP_HTTPRESPONSE_H
#define K_NET_HTTP_HTTPRESPONSE_H

#include <string>
#include "HttpVersion.h"

namespace K {

	class HttpResponse {

	public:

		/** ctor */
		HttpResponse() {;}


		/** get the response code */
		int getCode() const {return code;}

		/** get the provided http version */
		HttpVersion getVersion() const {return version;}


		/** set the response code */
		void setCode(const int code) {this->code = code;}

		/** set the to-be-used HTTP version */
		void setVersion(const HttpVersion version) {this->version = version;}


		/** get the response line as string: HTTP 200 OK */
		std::string getAsString() const {

			std::string ret; ret.reserve(32);
			ret += "HTTP/";

			switch(version) {
				case HttpVersion::HTTP_1_0:	ret += "1.0"; break;
				case HttpVersion::HTTP_1_1:	ret += "1.1"; break;
				case HttpVersion::HTTP_2:	ret += "2.0"; break;
			}

			ret += ' ';
			ret += std::to_string(code);
			ret += " TODO";
			ret += "\r\n";
			return ret;

		}

	private:

		/** the used http version */
		HttpVersion version;

		/** the response code */
		int code;



	};

}

#endif // K_NET_HTTP_HTTPRESPONSE_H
