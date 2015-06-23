/*
 * LineInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef LINEINPUTSTREAM_H_
#define LINEINPUTSTREAM_H_

#include "InputStream.h"

namespace K {

	class LineInputStream : public InputStream {
	public:

		/** ctor */
		LineInputStream(InputStream* is) : is(is), lastChar(0) {
			;
		}

		/** dtor */
		virtual ~LineInputStream() {
			;
		}

		/** read the whole file */
		std::string readAll() {
			std::string s = "";
			while(true) {
				uint8_t buf[4096];
				int len = read(buf, 4096);
				if (len == -1) {break;}
				s += std::string( (const char*) buf, len );
			}
			return s;
		}

		/** read next line from the underlying device */
		std::string readLine() {

			std::string ret;
			int byte;

			// build output
			while (true) {

				// get next byte
				byte = is->read();

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
			return is->read();
		}

		int read(uint8_t* data, unsigned int len) override {
			return is->read(data, len);
		}

		void close() override {
			is->close();
		}

		void skip(const uint64_t n) override {
			is->skip(n);
		}


	private:

		/** input stream. should be buffered! */
		InputStream* is;

		/** used to detect \r\n */
		int lastChar;

	};

}

#endif /* LINEINPUTSTREAM_H_ */
