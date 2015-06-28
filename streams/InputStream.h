/*
 * InputStream.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_

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
	virtual int read(uint8_t* data, unsigned int len) {
		unsigned int bytesRead = 0;
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
	int readFully(uint8_t* data, const unsigned int len) {
		unsigned int bytes = 0;
		while (bytes < len) {
			const int numRead = read(data+bytes, len-bytes);
			if (numRead == ERR_FAILED) {return ERR_FAILED;}
			bytes += numRead;
		}
		return bytes;
	}

	/** skip the next n bytes of the input */
	virtual void skip(const uint64_t n) = 0;

	/** close the input stream */
	virtual void close() = 0;

};

}


#endif /* INPUTSTREAM_H_ */
