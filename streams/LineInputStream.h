/*
 * LineInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef LINEINPUTSTREAM_H_
#define LINEINPUTSTREAM_H_

#include "BufferedInputStream.h"

namespace K {

class LineInputStream : public InputStream {
public:

	/** ctor */
	LineInputStream(BufferedInputStream& bis) : bis(bis), lastChar(0) {
		;
	}

	/** dtor */
	virtual ~LineInputStream() {
		;
	}

	/** read next line from the underlying device */
	std::string readLine() {

		std::string ret;
		int byte;

		// build output
		while (true) {

			// get next byte
			byte = bis.read();

			// check for EOF
			if (byte == -1) {break;}

			// split?
			if (byte == '\r') {break;}
			if (byte == '\n') {
				if (lastChar == '\r') {lastChar = byte; continue;}
				break;
			}

			// append current char
			ret += (char) byte;
			lastChar = byte;

		}

		lastChar = byte;

		// done
		return ret;

	}

	int read() override {
		return bis.read();
	}

	int read(uint8_t* data, unsigned int len) override {
		return bis.read(data, len);
	}

	void close() override {
		bis.close();
	}


private:

	/** buffered input stream */
	BufferedInputStream bis;

	/** used to detect \r\n */
	int lastChar;

};

}

#endif /* LINEINPUTSTREAM_H_ */
