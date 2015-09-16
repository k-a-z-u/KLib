/*
 * ByteArrayInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef BYTEARRAYINPUTSTREAM_H_
#define BYTEARRAYINPUTSTREAM_H_

#include "InputStreamPeek.h"
#include "StreamException.h"

namespace K {

class ByteArrayInputStream : public InputStreamPeek {

public:

	ByteArrayInputStream(const uint8_t* data, size_t len) : data(data), len(len) {
		;
	}

	int peek() override {
		if (len == 0) {return -1;}
		return *data;
	}

	virtual int read() override {
		if (len == 0) {return -1;}
		uint8_t ret = *data;
		--len;
		++data;
		return ret;
	}

	virtual ssize_t read(uint8_t* data, const size_t len) override {
		if (this->len == 0) {return -1;}
		ssize_t toRead = (this->len >= len) ? (len) : (this->len);
		memcpy (data, this->data, toRead);
		this->data += toRead;
		this->len -= toRead;
		return toRead;
	}

	void close() override {
		;
	}

	void skip(const size_t n) override {
		if (this->len < n) {throw StreamException("out of bounds while trying to skip some bytes");}
		this->data += n;
		this->len -= n;
	}

	/** get the number of bytes available for reading */
	size_t getNumAvailable() const {
		return this->len;
	}

private:

	/** the data buffer to read from */
	const uint8_t* data;

	/** the length of available data */
	size_t len;

};

}

#endif /* BYTEARRAYINPUTSTREAM_H_ */
