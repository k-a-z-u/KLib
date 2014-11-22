#ifndef K_NET_HTTP_HTTPREQUEST_H
#define K_NET_HTTP_HTTPREQUEST_H

#include <string>
#include "HttpException.h"
#include "HttpVersion.h"
#include "HttpHelper.h"

namespace K {

	class HttpRequest {

	public:

		/** empty ctor */
		HttpRequest() {;}


		/**
		 * ctor.
		 * create request from the first header line. e.g.
		 *	GET /index.html HTTP/1.1
		 */
		HttpRequest(const std::string& str) {

			// GET/POST/PATCH..
			const size_t pos1 = str.find(' ');
			if (pos1 == str.npos) {throw HttpException("invalid HTTP header detected");}
			method = HttpHelper::toUpper(str.substr(0, pos1));

			// the requested url
			const size_t pos2 = str.find(' ', pos1+1);
			if (pos2 == str.npos) {throw HttpException("invalid HTTP header detected");}
			url = str.substr(pos1+1, pos2-pos1-1);

			// http version
			const std::string tmp = str.substr(pos2+1);
			const size_t pos3 = tmp.find('/');
			if (pos3 == str.npos) {throw HttpException("invalid HTTP header detected");}
			const std::string sA = HttpHelper::toUpper(tmp.substr(0, pos3));
			const std::string sB = tmp.substr(pos3+1);

			// check the HTTP version
			if (sA != "HTTP") {throw HttpException("invalid HTTP header detected. invalid HTTP version.");}
			if		(sB == "1.0")	{version = HttpVersion::HTTP_1_0;}
			else if (sB == "1.1")	{version = HttpVersion::HTTP_1_1;}
			else if (sB == "2.0")	{version = HttpVersion::HTTP_2;}
			else					{throw HttpException("invalid HTTP version given: " + sB);}

		}


		/** get the request method (GET, POST, ..) */
		const std::string& getMethod() const {return method;}

		/** get the requested URL */
		const std::string& getURL() const {return url;}

		/** get the provided http version */
		HttpVersion getVersion() const {return version;}



		/** set the to-be-used HTTP version */
		void setVersion(const HttpVersion version) {this->version = version;}

		/** set the HTTP method */
		void setMethod(const std::string& method) {this->method = HttpHelper::toUpper(method);}

		/** set the URL */
		void setURL(const std::string& url) {this->url = url;}


		/** get the request line as string */
		std::string getAsString() const {
			std::string ret; ret.reserve(128);
			ret += method;
			ret += ' ';
			ret += url;
			ret += ' ';
			ret += "HTTP/";
			switch(version) {
				case HttpVersion::HTTP_1_0:	ret += "1.0"; break;
				case HttpVersion::HTTP_1_1:	ret += "1.1"; break;
				case HttpVersion::HTTP_2:	ret += "2.0"; break;
			}
			return ret;
		}


	private:

		/** the request method. GET, POST, .. */
		std::string method;

		/** the requested url */
		std::string url;

		/** the used http version */
		HttpVersion version;

	};

}

#endif // K_NET_HTTP_HTTPREQUEST_H
