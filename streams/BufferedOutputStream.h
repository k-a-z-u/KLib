/*
 * BufferedOutputStream.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef BUFFEREDOUTPUTSTREAM_H_
#define BUFFEREDOUTPUTSTREAM_H_

#include "OutputStream.h"

namespace K {


class BufferedOutputStream : public OutputStream {

public:

	/** ctor */
	BufferedOutputStream(OutputStream& os, unsigned int blockSize = 4096) :
		os(os), blockSize(blockSize) {
		;
	}

	/** dtor */
	virtual ~BufferedOutputStream() {
		close();
	}

	virtual void write(uint8_t data) override {
		buffer.add(data);
		checkFull();
	}

	virtual void write(const uint8_t* data, unsigned int len) override {
		buffer.add(data, len);
		checkFull();
	}


	void flush() override {
		send();
		os.flush();
	}

	void close() override {
		send();
		os.close();
	}


private:

	/** send to underlying layer if block-size has been reached */
	void checkFull() {
		if (buffer.getNumUsed() <= blockSize) {return;}
		send();
	}

	/** send to underlying layer */
	void send() {
		if (!buffer.getNumUsed()) {return;}
		os.write(buffer.getData(), buffer.getNumUsed());
		buffer.clear();
	}

	/** the stream to write to */
	OutputStream& os;

	/** the block size for flushing */
	unsigned int blockSize;

	/** the internal buffer */
	Buffer<uint8_t> buffer;

};

}


#endif /* BUFFEREDOUTPUTSTREAM_H_ */
