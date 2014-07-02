/*
 * BufferedInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef BUFFEREDINPUTSTREAM_H_
#define BUFFEREDINPUTSTREAM_H_

#include <vector>
#include <cstdint>
#include <cstring>
#include "InputStream.h"
#include "Buffer.h"
#include "StreamException.h"

namespace K {

/**
 * adds additional buffering to an input stream
 */
class BufferedInputStream : public InputStream {

public:

	/** ctor */
	BufferedInputStream(InputStream& is, unsigned int blockSize = 4096) :
		is(is), blockSize(blockSize), eof(false) {
		;
	}

	/** dtor */
	virtual ~BufferedInputStream() {
		close();
	}

	int read() override {
		fillBuffer(1);
		if (eof && buffer.empty()) {return -1;}
		return buffer.get();
	}

	/** read the given number of bytes into the buffer */
	int read(uint8_t* data, unsigned int len) override {
		fillBuffer(len);
		if (eof && buffer.empty()) {return -1;}
		int toRead = (len < buffer.getNumUsed()) ? (len) : (buffer.getNumUsed());
		memcpy(data, &buffer[0], toRead);
		buffer.remove(toRead);
		return toRead;
	}

	void close() override {
		is.close();
	}

	void skip(const uint64_t n) override {
		(void) n;
		throw StreamException("BufferedInputStream.skip() not yet implemented");
	}

private:

	/** ensure the buffer contains the given number of bytes */
	void fillBuffer(unsigned int needed) {

		// buffer already contains enough bytes
		if (buffer.getNumUsed() >= needed) {return;}

		// how many bytes to fetch
		int toFetch = (needed < blockSize) ? (blockSize) : (needed);

		// make space for those bytes
		buffer.resize(buffer.getNumUsed() + toFetch);

		// fetch from underlying layer
		int fetched = is.read(buffer.getFirstFree(), toFetch);
		if (fetched == -1) {eof = true; return;}

		buffer.setNumUsed(buffer.getNumUsed() + fetched);


	}

	/** the underlying stream */
	InputStream& is;

	/** the internal buffer */
	Buffer<uint8_t> buffer;

	/** the number of bytes to read every time */
	unsigned int blockSize;

	/** eof from underlying layer? */
	bool eof;

};

}

#endif /* BUFFEREDINPUTSTREAM_H_ */
