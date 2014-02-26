/*
 * SocketException.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef SOCKETEXCEPTION_H_
#define SOCKETEXCEPTION_H_

#include <exception>
#include <string>

/**
 * exception handling within the sockets backend
 */
class SocketException : public std::exception {

public:

	/** ctor */
	SocketException(const std::string& msg) : msg(msg) {
		;
	}

	const char* what() const throw() override {
		return msg.c_str();
	}

private:

	/** error message */
	std::string msg;

};


#endif /* SOCKETEXCEPTION_H_ */
