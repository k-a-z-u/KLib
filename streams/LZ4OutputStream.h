/*
 * LZ4OutputStream.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef LZ4OUTPUTSTREAM_H_
#define LZ4OUTPUTSTREAM_H_

#include "lz4/lz4.h"
#include "lz4/lz4.hc"
#include "OutputStream.h"
#include "Buffer.h"

namespace K {


/**
 * stream to compress data using LZ4
 */
class LZ4OutputStream : public OutputStream {

public:

	/**
	 * ctor
	 * @param os the OutputStream to write the compressed data to
	 * @param bufferSize the number of bytes to buffer before compressing the data
	 */
	LZ4OutputStream(OutputStream& os, unsigned int bufferSize = 4096) : os(os), bufferSize(bufferSize) {
		;
	}

	/** dtor */
	~LZ4OutputStream() {
		close();
	}


	void write(uint8_t data) override {
		buffer.add(data);
		checkCompress();
	}

	void write(const uint8_t* data, const size_t len) override {
		buffer.add(data, len);
		checkCompress();
	}

	void flush() override {
		compress();
		os.flush();
	}

	void close() override {
		compress();
		os.close();
	}

private:

	/** buffer limit reached? -> compress */
	void checkCompress() {
		if (buffer.getNumUsed() < bufferSize) {return;}
		compress();
	}

	/** perform compression */
	void compress() {

		// nothing to compress?
		if (buffer.empty()) {return;}

		// ensure compression buffer provides enough space
		unsigned int maxCompSize = LZ4_compressBound(buffer.getNumUsed());
		bufferComp.ensureMinSize(maxCompSize);

		// compress available input
		unsigned int outSize = LZ4_compress( (const char*) buffer.getData(), (char*) bufferComp.getData()+4, buffer.getNumUsed());
		if (outSize == 0) {throw "empty compression";}

		// remove available input
		buffer.clear();

		// add size-information for the number of compressed bytes to the stream
		*((int*)bufferComp.getData()) = outSize;

		// pass to next layer
		os.write(bufferComp.getData(), outSize+4);

	}

	/** the stream to write to */
	OutputStream& os;

	/** the number of bytes to buffer before compressing */
	unsigned int bufferSize;

	/** data buffer to enhance compression */
	Buffer<uint8_t> buffer;

	/** compression buffer */
	Buffer<uint8_t> bufferComp;

};


}


#endif /* LZ4OUTPUTSTREAM_H_ */
