/*
 * GzipInputStream.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef GZIPINPUTSTREAM_H_
#define GZIPINPUTSTREAM_H_

#ifdef WITH_ZLIB

#include "GzipOutputStream.h"
#include "InputStream.h"
#include "Buffer.h"
#include <zlib.h>

namespace K {

/**
 * stream to read gzip compressed data and provide
 * the uncompressed equivalent
 */
class GzipInputStream : public InputStream {

public:

	/**
	 * ctor
	 * @param is the (compressed) InputStream to read from
	 */
	GzipInputStream(InputStream& is,
			GzipOutputStreamHeader header = GzipOutputStreamHeader::MODE_DEFLATE) :
				is(is), eof(false) {
		setup(header);
		bufferIn.resize(4*1024);
	}

	/** dtor */
	~GzipInputStream() {
		inflateEnd(&stream);
	}

	int read() override {
		uint8_t data;
		while(true) {
			int ret = read(&data, 1);
			if (ret == -1) {return -1;}
			if (ret != 0) {break;}
		}
		return data;
	}

	int read(uint8_t* data, unsigned int len) {

		// fill the input buffer
		if (bufferIn.getNumUsed() < 1024 && !eof) {
			int read = is.read(bufferIn.getFirstFree(), bufferIn.getNumFree());
			if (read > 0) {bufferIn.setNumUsed(bufferIn.getNumUsed() + read);}
			if (read == -1) {eof = true;}
		}

		// what to decompress
		stream.next_in = &bufferIn[0];
		stream.avail_in = bufferIn.getNumUsed();

		// where to decompress to
		stream.next_out = data;//bufferDst.getFirstFree();
		stream.avail_out = len;//bufferDst.getNumFree();

		// perform decompresion
		int ret = inflate(&stream, Z_NO_FLUSH);

		// decompression error?
		if (ret != Z_OK && ret != Z_STREAM_END) {throw "error while decompressing data";}

		// number of decompressed bytes
		unsigned int decompressed = len - stream.avail_out;//bufferDst.getNumFree() - stream.avail_out;

		// end of stream?
		if (ret == Z_STREAM_END && decompressed == 0) {	return -1; }

		// decompression buffer too small?
		if (decompressed == 0 && ret == Z_OK) {throw "decompression buffer too small";}

		// update input buffer
		unsigned int usedInput = bufferIn.getNumUsed() - stream.avail_in;
		bufferIn.remove(usedInput);

		// done
		return decompressed;

	}

	void close() override {
		is.close();
	}

private:


	/** setup the decompressor */
	void setup(GzipOutputStreamHeader header) {

		// reset
		memset(&stream, 0, sizeof(stream));

		// use zlib header or gzip header?
		int windowBits = MAX_WBITS;
		if (header == GzipOutputStreamHeader::MODE_GZIP) {windowBits |= 16;}

		// init
		int ret = inflateInit2(&stream, windowBits);
		if (ret != Z_OK) {throw "error initializing decompression";}

	}


	/** the stream to read from */
	InputStream& is;

	/** the InputStream buffer */
	Buffer<uint8_t> bufferIn;

	/** the decompression stream */
	z_stream stream;

	/** end of file reached? */
	bool eof;

};

}

#endif

#endif /* GZIPINPUTSTREAM_H_ */
