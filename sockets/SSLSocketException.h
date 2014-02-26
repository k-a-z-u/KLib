/*
 * SSLSocketException.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef SSLSOCKETEXCEPTION_H_
#define SSLSOCKETEXCEPTION_H_

/**
 * special exceptions for errors within the SSL backend
 * of the socket classes.
 */
class SSLSocketException : public std::exception {

public:

	/** ctor */
	SSLSocketException(const std::string& msg) : msg(msg) {
		this->msg += "\n";
		this->msg += ERR_error_string(ERR_get_error(), nullptr);
	}

	const char* what() const throw() override {
		return msg.c_str();
	}

private:

	/** error message */
	std::string msg;

};


#endif /* SSLSOCKETEXCEPTION_H_ */
