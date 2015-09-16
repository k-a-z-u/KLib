/*
 * InputStream.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef K_STREAMS_INPUTSTREAM_H_
#define K_STREAMS_INPUTSTREAM_H_

#include <cstdint>
#include <string>
#include <iostream>

namespace K {


class InputStream {

public:

	/** reading failed completely */
	static constexpr int ERR_FAILED = -1;

	/** reading failed as nothing is available. try again */
	static constexpr int ERR_TRY_AGAIN = -2;


	/** dtor */
	virtual ~InputStream() {;}

	/**
	 * read one byte from the underlying device.
	 * returns ERR_FAILED if reading failed
	 * returns ERR_TRY_AGAIN if nothing was available (but could be in the future)
	 */
	virtual int read() = 0;

	/**
	 * read the given number of bytes into the buffer.
	 * returns the number of read bytes (if everything was fine)
	 * returns 0 if nothing was available for reading (but could be in the future)
	 * returns ERR_FAILED if reading failed
	 */
	virtual ssize_t read(uint8_t* data, size_t len) {
		size_t bytesRead = 0;
		while (--len) {
			int ret = read();
			if (ret == -1) {break;}
			*data = (uint8_t) ret;
			++data;
			++bytesRead;
		}
		return bytesRead;
	}

	/**
	 * try the read the given number of bytes.
	 * will block until everything could be read.
	 * returns the number of read bytes (if everything was fine)
	 * returns ERR_FAILED if reading failed
	 */
	ssize_t readFully(uint8_t* data, const size_t len) {
		size_t bytes = 0;
		while (bytes < len) {
			const ssize_t numRead = read(data+bytes, len-bytes);
			if (numRead == ERR_FAILED) {return ERR_FAILED;}
			if (numRead == ERR_TRY_AGAIN) {continue;}
			bytes += numRead;
		}
		return bytes;
	}

	/** skip the next n bytes of the input */
	virtual void skip(const size_t n) = 0;

	/** close the input stream */
	virtual void close() = 0;

};

}


#endif /* K_STREAMS_INPUTSTREAM_H_ */
