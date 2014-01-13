/*
 * ByteBuffer.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#include <cstdint>

namespace K {

class ByteBuffer {

public:

	/** ctor */
	ByteBuffer();

	/** dtor */
	~ByteBuffer();

	/** append the given data */
	void write(uint8_t* data, int len);

	/** get the number of free bytes */
	unsigned int freeBytes();

private:

	/** make sure the buffer has at least the given size */
	void resize(unsigned int size);

	/** ensure the buffer provides space for the given number of bytes */
	void ensureSpaceFor(unsigned int size);


	/** the data buffer */
	uint8_t* data;

	/** the total size of the buffer */
	unsigned int size;

	/** the number of used bytes */
	unsigned int used;

};

}

#endif /* BYTEBUFFER_H_ */
