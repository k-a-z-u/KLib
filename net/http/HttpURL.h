#ifndef HTTPURL_H
#define HTTPURL_H

#include <string>
#include "../../Assertions.h"

namespace K {

	class HttpURL {

	private:

		/** the protocol: http/https/... */
		std::string protocol;

		/** hostname: e.g. www.example.org */
		std::string host;

		/** port-number on the host. usually: 80 */
		uint16_t port;

		/** the file on the server: e.g. /website/index.html */
		std::string file;


	public:

		/** empty ctor */
		HttpURL() : port(80) {
			;
		}

		/**
		 * ctor
		 * takes a complete URL: https://www.example.org:8080/website/index.html
		 */
		explicit HttpURL(const std::string& url) : port(80) {

			// get the protocol (http/https/...)
			const ssize_t posProtoEnd = url.find('/');
			setProtocol(url.substr(0, posProtoEnd-1));

			// get the host (and port, if present)
			const ssize_t posHostEnd = url.find('/', posProtoEnd+3);
			setHost(url.substr(posProtoEnd+2, posHostEnd-posProtoEnd-2));

			// set the file
			setFile(url.substr(posHostEnd));

		}

		/** set the filename. e.g. "/website/index.html" */
		void setFile(const std::string& file) {
			this->file = file;
		}

		/** set the hostname. may contain a port-number like "www.example.org:8080" */
		void setHost(const std::string& host) {
			const size_t pos = host.find(':');
			if (pos == host.npos) {
				this->host = host;
			} else {
				this->host = host.substr(0, pos);
				this->port = (uint16_t) std::stoi(host.substr(pos+1));
			}
		}

		/** set the protocol (http/https/...) */
		void setProtocol(const std::string& protocol) {
			this->protocol = protocol;
		}

		/** set the port-number */
		void setPort(const uint16_t port) {
			this->port = port;
		}

		/** get the used protocol (http/https/..) */
		const std::string& getProtocol() const {
			return protocol;
		}

		/** get the hostname only (without port) */
		const std::string& getHost() const {
			return host;
		}

		/** get the port-number */
		uint16_t getPort() const {
			return port;
		}

		/** get the hostname including the port (if not 80) */
		std::string getHostWithPort() const {
			return host + ((port == 80) ? ("") : (":"+std::to_string(port)));
		}

		/** get the filename only */
		const std::string& getFile() const {
			return file;
		}

		/** get the complete URL as string */
		std::string getURL() const {
			_assertNotEmpty(protocol, "protocol is missing");
			_assertNotEmpty(host, "hostname is missing");
			_assertNotEmpty(file, "filename is missing");
			return getProtocol() + "://" + getHostWithPort() + getFile();
		}

		/** append URL to string */
		friend std::string operator + (const std::string& s, const HttpURL& url) {
			return s + url.getURL();
		}

	};

}

#endif // HTTPURL_H
