/*
 * ByteArrayInOutStream.h
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */

#include "InputStream.h"
#include "OutputStream.h"
#include "Buffer.h"
#include "StreamException.h"

namespace K {

/**
 * provides both, an input and an output stream
 * to write data to and read from.
 *
 * e.g. for testing purpose
 */
class ByteArrayInOutStream : virtual public InputStream, virtual public OutputStream {

public:

	/** ctor */
	ByteArrayInOutStream() {
		;
	}

	/** dtor */
	~ByteArrayInOutStream() {

	}


	int read() override {
		if (buffer.empty()) {return -1;}
		int ret = buffer.get();
		return ret;
	}

	ssize_t read(uint8_t* data, const size_t len) override {
		if (buffer.empty()) {return -1;}
		const ssize_t toRead = (buffer.getNumUsed() >= len) ? (len) : (buffer.getNumUsed());
		memcpy(data, buffer.getData(), toRead);
		buffer.remove(toRead);
		return toRead;
	}


	void write(uint8_t data) override {
		buffer.add(data);
	}

	void write(const uint8_t* data, const size_t len) override {
		buffer.add(data, len);
	}


	void close() override {;}

	void flush() override {;}

	void skip(const uint64_t n) override {
		buffer.remove((uint32_t) n);
		//throw StreamException("ByteArrayInOutStream.skip() not yet implemented");
	}

	/** get the number of available bytes */
	size_t length() const {
		return buffer.getNumUsed();
	}

private:

	/** the internal buffer */
	Buffer<uint8_t> buffer;

};

}
