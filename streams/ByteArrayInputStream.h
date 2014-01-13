/*
 * ByteArrayInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef BYTEARRAYINPUTSTREAM_H_
#define BYTEARRAYINPUTSTREAM_H_

#include "InputStream.h"

namespace K {

class ByteArrayInputStream : public InputStream {

public:

	ByteArrayInputStream(const uint8_t* data, unsigned int len) : data(data), len(len) {
		;
	}

	virtual int read() override {
		if (len == 0) {return -1;}
		uint8_t ret = *data;
		--len;
		++data;
		return ret;
	}

	virtual int read(uint8_t* data, unsigned int len) {
		if (this->len == 0) {return -1;}
		unsigned int toRead = (this->len >= len) ? (len) : (this->len);
		memcpy (data, this->data, toRead);
		this->data += toRead;
		this->len -= toRead;
		return toRead;
	}

	void close() override {
		;
	}

	/** get the number of bytes available for reading */
	unsigned int getNumAvailable() const {
		return this->len;
	}

private:

	/** the data buffer to read from */
	const uint8_t* data;

	/** the length of available data */
	unsigned int len;

};

}

#endif /* BYTEARRAYINPUTSTREAM_H_ */
