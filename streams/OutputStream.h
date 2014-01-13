/*
 * OutputStream.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef OUTPUTSTREAM_H_
#define OUTPUTSTREAM_H_

#include <cstdint>

namespace K {

class OutputStream {

public:

	/** dtor */
	virtual ~OutputStream() {;}

	/** write one byte */
	virtual void write(uint8_t data) = 0;

	/** write multiple bytes */
	virtual void write(uint8_t* data, unsigned int len) = 0;

	/** flush the given data to the underlying layer */
	virtual void flush() = 0;

	/** close the output stream */
	virtual void close() = 0;

};

}

#endif /* OUTPUTSTREAM_H_ */
