#ifndef K_NET_HTTP_HTTPRESPONSE_H
#define K_NET_HTTP_HTTPRESPONSE_H

#include <string>
#include "HttpVersion.h"
#include "HttpHeader.h"

#include "../../streams/LineInputStream.h"

namespace K {

	class HttpResponse {

	private:

		/** the used http version */
		HttpVersion version;

		/** the response code */
		int code;

		/** the message after the response code */
		std::string codeMsg;

		/** the response header */
		HttpHeader header;


	public:

		/** empty ctor */
		HttpResponse() {;}

		/** configured ctor */
		HttpResponse(const HttpVersion version, const int code, const std::string codeMsg = "") :
			version(version), code(code), codeMsg(codeMsg) {
			;
		}

		/** ctor: from input-stream */
		HttpResponse(LineInputStream& lis) {

			// read request line
			parseFirstLine(lis.readLine());

			// read the HTTP header
			while (true) {
				const std::string line = lis.readLine();
				if (line.empty()) {break;}
				header.addLine(line);
			}

		}

		/** get the response code */
		int getCode() const {return code;}

		/** get the message behind the code */
		const std::string& getCodeMessage() {return codeMsg;}

		/** get the provided http version */
		HttpVersion getVersion() const {return version;}


		/** set the response code */
		void setCode(const int code) {this->code = code;}

		/** set the response code and message */
		void setCode(const int code, const std::string& codeMsg) {this->code = code; this->codeMsg = codeMsg;}

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

		/** get the response header */
		HttpHeader& getHeader() {return header;}

	private:

		/** parse the "GET /index.html HTTP/1.1" line */
		void parseFirstLine(const std::string& str) {

			// http version
			const size_t pos1 = str.find(' ');
			const std::string tmp = str.substr(0, pos1);
			const size_t pos2 = tmp.find('/');
			if (pos2 == str.npos) {throw HttpException("invalid HTTP header detected");}
			const std::string sA = HttpHelper::toUpper(tmp.substr(0, pos2));
			const std::string sB = tmp.substr(pos2+1);

			// check the HTTP version
			if (sA != "HTTP") {throw HttpException("invalid HTTP header detected. invalid HTTP version.");}
			if		(sB == "1.0")	{version = HttpVersion::HTTP_1_0;}
			else if (sB == "1.1")	{version = HttpVersion::HTTP_1_1;}
			else if (sB == "2.0")	{version = HttpVersion::HTTP_2;}
			else					{throw HttpException("invalid HTTP version given: " + sB);}

			// code number
			const size_t pos3 = str.find(' ', pos1+1);
			const std::string codeNr = str.substr(pos1+1, pos3-pos1);
			const std::string codeStr = str.substr(pos3+1);
			setCode(std::stoi(codeNr), codeStr);

		}

	};

}

#endif // K_NET_HTTP_HTTPRESPONSE_H
