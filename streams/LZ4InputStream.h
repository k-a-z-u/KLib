/*
 * LZ4InputStream.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef LZ4INPUTSTREAM_H_
#define LZ4INPUTSTREAM_H_

#include "lz4/lz4.h"
#include "InputStream.h"
#include "Buffer.h"

namespace K {

/**
 * stream to decompress LZ4-compressed input data
 */
class LZ4InputStream : public InputStream {

public:

	/** ctor */
	LZ4InputStream(InputStream& is) : is(is), eof(false) {
		bufferDecomp.resize(4*1024);
	}

	/** dtor */
	~LZ4InputStream() {

	}

	int read() override {
		if (bufferDecomp.empty()) {decompressBlock();}
		if (bufferDecomp.empty()) {return -1;}
		return bufferDecomp.get();
	}

	int read(uint8_t* data, unsigned int len) override {
		if (bufferDecomp.empty()) {decompressBlock();}
		if (bufferDecomp.empty()) {return -1;}
		int toRead = (len <= bufferDecomp.getNumUsed()) ? (len) : (bufferDecomp.getNumUsed());
		memcpy(data, bufferDecomp.getData(), toRead);
		bufferDecomp.remove(toRead);
		return toRead;
	}

	void close() override {
		is.close();
	}

private:

	/**
	 * read the next compressed block from the underlying input stream
	 * and decompress it into a temporal buffer.
	 */
	void decompressBlock() {

		// read block length
		int blockSize;
		int read = is.readFully( (uint8_t*)&blockSize, 4);
		if (read == -1) {return;}
		//std::cout << "next block: " << blockSize << std::endl;

		// read compressed data
		uint8_t buf[blockSize];
		read = is.readFully(buf, blockSize);
		if (read != blockSize) {throw "could not read block";}

		// decompress
		again:
		int decomp = LZ4_decompress_safe( (const char*) buf, (char*) bufferDecomp.getData(), blockSize, bufferDecomp.getSize() );


		// check whether the buffer was able to catch all decompressed bytes
		// if not: resize the buffer and try again
		if (decomp < 0 || (unsigned int)decomp == bufferDecomp.getSize()) {

			// prevent growing to infinity
			if (bufferDecomp.getSize() > 1024*1024*16) {
				throw "stream corrupted??";
			}

			// resize the buffer
			bufferDecomp.resize(bufferDecomp.getSize()*2);
			std::cout << "growing to " << bufferDecomp.getSize() << std::endl;

			// try decompression
			goto again;

		}

		// everything fine
		bufferDecomp.setNumUsed(decomp);

	}


	/** the compressed InputStream to read from */
	InputStream& is;

	/** the decompression buffer */
	Buffer<uint8_t> bufferDecomp;

	/** eof reached? */
	bool eof;

};

}



#endif /* LZ4INPUTSTREAM_H_ */
