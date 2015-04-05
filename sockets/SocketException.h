/*
 * SocketException.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef K_SOCKET_SOCKETEXCEPTION_H_
#define K_SOCKET_SOCKETEXCEPTION_H_

#include <exception>
#include <string>
#include <errno.h>
#include <string.h>

/**
 * exception handling within the sockets backend
 */
class SocketException : public std::exception {

public:

	/** ctor */
	SocketException(const std::string& msg) : msg(msg) {
		;
	}

	/** ctor with string and errno */
	SocketException(const std::string& msg, const int errnum) : msg(msg) {
		this->msg += ": ";
		this->msg += strerror(errnum);
	}

	const char* what() const throw() override {
		return msg.c_str();
	}

private:

	/** error message */
	std::string msg;

};


#endif /* K_SOCKET_SOCKETEXCEPTION_H_ */
