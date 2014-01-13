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

	/** dtor */
	virtual ~InputStream() {;}

	/** read one byte from the underlying device. returns -1 if nothin is available */
	virtual int read() = 0;

	/** read the given number of bytes into the buffer */
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

	int readFully(uint8_t* data, unsigned int len) {
		unsigned int bytes = 0;
		while (bytes < len) {
			int numRead =  read(data+bytes, len-bytes);
			if (numRead == -1) {return -1;}
			bytes += numRead;
		}
		return bytes;
	}

	/** close the input stream */
	virtual void close() = 0;

};

}


#endif /* INPUTSTREAM_H_ */
