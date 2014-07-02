#ifndef K_STREAMS_STREAMEXCEPTION_H
#define K_STREAMS_STREAMEXCEPTION_H

#include <exception>

namespace K {

	/**
	 * exception handling within streams
	 */
	class StreamException : public std::exception {
	public:
		StreamException(const std::string& msg) : msg(msg) {;}
		const char* what() const throw() {return msg.c_str();}
	private:
		std::string msg;
	};

}

#endif // STREAMEXCEPTION_H
