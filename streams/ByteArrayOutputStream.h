/*
 * ByteArrayOutputStream.h
 *
 *  Created on: Dec 17, 2013
 *      Author: kazu
 */

#ifndef BYTEARRAYOUTPUTSTREAM_H_
#define BYTEARRAYOUTPUTSTREAM_H_

#include "OutputStream.h"
#include "Buffer.h"

namespace K {

/**
 * OutputStream to write to a byte array
 */
class ByteArrayOutputStream : public OutputStream {


public:

	/** ctor */
	ByteArrayOutputStream() {;}


	void write(uint8_t data) override {
		buffer.add(data);
	}

	void write(const uint8_t* data, const size_t len) override {
		buffer.add(data, len);
	}

	void flush() override {
		;
	}

	void close() override {
		;
	}

	/** get internal data as byte pointer */
	const uint8_t* getData() {
		return buffer.getData();
	}

	/** get the number of available bytes */
	size_t getDataLength() {
		return buffer.getNumUsed();
	}


private:

	/** the internal data buffer */
	Buffer<uint8_t> buffer;

};

}


#endif /* BYTEARRAYOUTPUTSTREAM_H_ */
