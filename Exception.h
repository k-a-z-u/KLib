/*
 * Exception.h
 *
 *  Created on: Dec 28, 2013
 *      Author: kazu
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_


namespace K {

/**
 * base-class for exceptions
 */
class Exception : public std::exception {

public:

	/** ctor */
	Exception(const std::string& msg) : msg(msg) {;}

	const char* what() const throw() override {
		return msg.c_str();
	}

private:

	/** the error message */
	std::string msg;

};

}


#endif /* EXCEPTION_H_ */
