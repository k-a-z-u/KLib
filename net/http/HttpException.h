#ifndef K_NET_HTTP_HTTPEXCEPTION_H
#define K_NET_HTTP_HTTPEXCEPTION_H

#include <exception>

namespace K {

	class HttpException : public std::exception {
	public:
		HttpException(const std::string& str) : str(str) {;}
		const char* what() const throw() {return str.c_str();}
	private:
		std::string str;
	};

}

#endif // K_NET_HTTP_HTTPEXCEPTION_H
