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
	ByteBuffer(unsigned int initialSize = 4096) {
		resize(initialSize);
	}

	/** dtor */
	~ByteBuffer() {
		if(data) {free(data);}
	}

	/** append the given data */
	void write(uint8_t* data, int len) {
		ensureSpaceFor(len);
		memcpy(this->data+used, data, len);
		this->used += len;
	}

	/** get the number of free bytes */
	unsigned int freeBytes() {
		return size - used;
	}

private:

	/** make sure the buffer has at least the given size */
	void resize(unsigned int size) {
		data = (uint8_t*) realloc(data, size);
		this->size = size;
	}

	/** ensure the buffer provides space for the given number of bytes */
	void ensureSpaceFor(unsigned int size) {
		unsigned int free = freeBytes();
		if (free > size) {return;}
		unsigned int newSize = (this->size + size) * 2;
		resize(newSize);
	}


	/** the data buffer */
	uint8_t* data;

	/** the total size of the buffer */
	unsigned int size;

	/** the number of used bytes */
	unsigned int used;

};

}

#endif /* BYTEBUFFER_H_ */
