#ifndef K_NET_HTTP_HTTPREQUEST_H
#define K_NET_HTTP_HTTPREQUEST_H

#include <string>
#include "HttpException.h"
#include "HttpEnums.h"
#include "HttpHelper.h"
#include "HttpURL.h"
#include "HttpRequestResponse.h"

#include "../../streams/ByteArrayInputStream.h"
#include "../../streams/LineInputStream.h"

namespace K {

	class HttpRequest : public HttpRequestResponse {

	private:

		/** the parsed url */
		HttpURL url;

		/** the request method. GET, POST, .. */
		std::string method;

		/** the used http version */
		HttpVersion version;


	public:

		/** empty ctor */
		HttpRequest() {;}

		/**
		 * ctor.
		 * create request from the HTTP-Header
		 * e.g. "GET /index.html HTTP/1.1\r\nConnection: close\r\n\r\n"
		 * its also possible to only provide the first line ("GET...")
		 * without the following header fields. in this case, the header-fields
		 * remain empty.
		 */
		HttpRequest(const std::string& str) {
			ByteArrayInputStream bais((uint8_t*)str.data(), str.length());
			LineInputStream lis(&bais);
			parse(&lis);
		}

		/**
		 * ctor.
		 * create request by building the first header line.
		 * e.g. GET /index.html HTTP/1.1.
		 * the following header-fields remain empty
		 */
		HttpRequest(const std::string& url, const std::string& method, const HttpVersion version) : url(HttpURL(url)), method(method), version(version) {
			header.add("Host", this->url.getHostWithPort());
		}

		/**
		 * ctor.
		 * parse an http-request (first-line + header)
		 * from the given LineInputStream
		 */
		HttpRequest(LineInputStream* lis) {
			parse(lis);
		}



		/** get the request method (GET, POST, ..) */
		const std::string& getMethod() const {return method;}

		/** get the requested URL */
		HttpURL& getURL() {return url;}

		/** get the provided http version */
		HttpVersion getVersion() const {return version;}



		/** set the to-be-used HTTP version */
		void setVersion(const HttpVersion version) {this->version = version;}

		/** set the HTTP method */
		void setMethod(const std::string& method) {this->method = HttpHelper::toUpper(method);}


		/** get the request header's first line as string */
		std::string getFirstLine() const {
			std::string ret; ret.reserve(128);
			ret += method;
			ret += ' ';
			ret += url.getFile();
			ret += ' ';
			ret += "HTTP/";
			switch(version) {
				case HttpVersion::HTTP_1_0:	ret += "1.0"; break;
				case HttpVersion::HTTP_1_1:	ret += "1.1"; break;
				case HttpVersion::HTTP_2:	ret += "2.0"; break;
			}
			return ret;
		}

		/** get the complete request header as string */
		std::string getRequestHeader() const {
			return getFirstLine() + "\r\n" + header.getAsString() + "\r\n";
		}

	private:

		/** parse a complete HTTP-header from the given line-input-stream */
		void parse(LineInputStream* lis) {

			// read request line
			parseFirstLine(lis->readLine());

			// read the HTTP header
			while (true) {
				const std::string line = lis->readLine();
				if (line.empty()) {break;}
				header.addLine(line);
			}

			// set the requested host
			// TODO: check whether this worked?!
			url.setHost(header.get("host"));

		}

		/** parse the "GET /index.html HTTP/1.1" line */
		void parseFirstLine(const std::string& str) {

			// GET/POST/PATCH..
			const size_t pos1 = str.find(' ');
			if (pos1 == str.npos) {throw HttpException("invalid HTTP request detected");}
			method = HttpHelper::toUpper(str.substr(0, pos1));

			// the requested url
			const size_t pos2 = str.find(' ', pos1+1);
			if (pos2 == str.npos) {throw HttpException("invalid HTTP request detected");}
			url.setFile(str.substr(pos1+1, pos2-pos1-1));

			// http version
			const std::string tmp = str.substr(pos2+1);
			const size_t pos3 = tmp.find('/');
			if (pos3 == str.npos) {throw HttpException("invalid HTTP request detected");}
			const std::string sA = HttpHelper::toUpper(tmp.substr(0, pos3));
			const std::string sB = tmp.substr(pos3+1);

			// check the HTTP version
			if (sA != "HTTP") {throw HttpException("invalid HTTP request detected. invalid HTTP version.");}
			if		(sB == "1.0")	{version = HttpVersion::HTTP_1_0;}
			else if (sB == "1.1")	{version = HttpVersion::HTTP_1_1;}
			else if (sB == "2.0")	{version = HttpVersion::HTTP_2;}
			else					{throw HttpException("invalid HTTP version given: " + sB);}

		}

	};

}

#endif // K_NET_HTTP_HTTPREQUEST_H
