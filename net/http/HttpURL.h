#ifndef HTTPURL_H
#define HTTPURL_H

#include <string>

namespace K {

	class HttpURL {

	private:

		std::string protocol;
		std::string host;
		std::string file;
		int port = 80;

	public:

		HttpURL() {;}

		explicit HttpURL(const std::string& url) {

			int posProtoEnd = url.find('/');
			protocol = url.substr(0, posProtoEnd-1);

			int posHostEnd = url.find('/', posProtoEnd+3);
			host = url.substr(posProtoEnd+2, posHostEnd-posProtoEnd-2);

			file = url.substr(posHostEnd);

			int i = 0;

		}

		void setFile(const std::string& file) {this->file = file;}
		void setHost(const std::string& host) {this->host = host;}

		const std::string& getProtocol() const {return protocol;}
		const std::string& getHost() const {return host;}
		const std::string& getFile() const {return file;}
		const int getPort() const {return port;}

	};

}

#endif // HTTPURL_H
