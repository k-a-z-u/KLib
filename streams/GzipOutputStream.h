/*
 * GzipOutputStream.h
 *
 *  Created on: Dec 17, 2013
 *      Author: kazu
 */

#ifndef GZIPOUTPUTSTREAM_H_
#define GZIPOUTPUTSTREAM_H_

#ifdef WITH_ZLIB

#include "OutputStream.h"
#include <zlib.h>

namespace K {

/** the header format to use */
enum class GzipOutputStreamHeader {

	/** use zlib's deflate header */
	MODE_DEFLATE,

	/** use gzip's header */
	MODE_GZIP,

};

/**
 * An OutputStream compressing all provided data-chunks
 * using GZIP
 */
class GzipOutputStream : public OutputStream {

public:

	/**
	 * ctor
	 * @param os the stream to send the compressed data to
	 * @param level the compression level between 1 (fastest) and 9 (best). default: -1
	 */
	GzipOutputStream(OutputStream& os,
			GzipOutputStreamHeader header = GzipOutputStreamHeader::MODE_DEFLATE,
			int level = Z_DEFAULT_COMPRESSION) : os(os) {

		settings.level = level;
		settings.header = header;

		setup();
		buffer.data = (uint8_t*) malloc(buffer.size);
		if (!buffer.data) {throw "error while allocating buffer";}

	}

	/** dtor */
	~GzipOutputStream() {
		free(buffer.data); buffer.data = nullptr;
		deflateEnd(&stream);
	}


	void write(uint8_t data) override {
		write(&data, 1);
	}

	void write(uint8_t* data, unsigned int len) override {

		// what to compress
		stream.next_in = (unsigned char*) data;
		stream.avail_in = len;

		compress(Z_NO_FLUSH);

	}

	void flush() override {
		compress(Z_SYNC_FLUSH);
		os.flush();
	}

	void close() override {
		compress(Z_FINISH);
		os.close();
	}


private:

	void compress(int flush) {

		unsigned int compressedBytes;

		// run until done
		do {

			//std::cout << stream.avail_in << " : ";
			// compress to output buffer
			stream.next_out = (unsigned char*) buffer.data;
			stream.avail_out = buffer.size;

			// compress
			int ret = deflate(&stream, flush);
			if (ret == Z_STREAM_ERROR) {throw "error while compressing data chunk";}

			// pipe all compressed bytes to next layer
			compressedBytes = buffer.size - stream.avail_out;
			if (compressedBytes) {
				os.write(buffer.data, compressedBytes);
			}
			//std::cout << stream.avail_in << "     " << compressedBytes << std::endl;

		} while (stream.avail_in || compressedBytes);

	}

#define GZIP_ENCODING 16

	/** initialize the stream */
	void setup() {

		// reset
		memset(&stream, 0, sizeof(stream));

		// use zlib header or gzip header?
		int windowBits = MAX_WBITS;
		if (settings.header == GzipOutputStreamHeader::MODE_GZIP) {windowBits |= 16;}

		// init
		int ret = deflateInit2(&stream, settings.level, Z_DEFLATED, windowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
		if (ret != Z_OK) {throw "error while initializing gzip";}

	}

	/** the next layer to write to */
	OutputStream& os;

	/** zlib struct */
	z_stream stream;

	/** compression settings */
	struct {

		/** the compresion level */
		int level;

		/** the file header */
		GzipOutputStreamHeader header;

	} settings;


	/** the temporal data buffer */
	struct {
		uint8_t* data;
		const int size = 64*1024;
	} buffer;


};

}

#endif

#endif /* GZIPOUTPUTSTREAM_H_ */
