/*
 * LineInputStream.h
 *
 *  Created on: Dec 16, 2013
 *      Author: kazu
 */

#ifndef LINEINPUTSTREAM_H_
#define LINEINPUTSTREAM_H_

#include "InputStream.h"
#include "InputStreamPeek.h"
#include "BufferedInputStream.h"
#include "IOException.h"

namespace K {

	class LineInputStream : public InputStream {

	public:

		/** ctor */
		LineInputStream(InputStreamPeek* is) : is(is), lastChar(0) {
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
				const ssize_t len = read(buf, 4096);
				if (len == ERR_FAILED) {break;}
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
				if (byte == ERR_FAILED) {
					if (ret.empty())	{
						throw IOException("error while reading next line from stream");}
					else				{break;}
				}

				// check no-data-yet
				if (byte == ERR_TRY_AGAIN) {continue;}

				// split?
				if (byte == '\r') {
					if (is->peek() == '\n') {is->read();}
					break;
				} else if (byte == '\n') {break;}
				//				if (byte == '\r') {break;}
//				if (byte == '\n') {
//					if (lastChar == '\r') {lastChar = byte; continue;}
//					break;
//				}

				// append current char
				ret += (char) byte;
				//lastChar = byte;

			}

			//lastChar = byte;

			// done
			return ret;

		}

		int read() override {
			return is->read();
		}

		ssize_t read(uint8_t* data, const size_t len) override {
			return is->read(data, len);
		}

		void close() override {
			is->close();
		}

		void skip(const size_t n) override {
			is->skip(n);
		}


	private:

		/** input stream. should be buffered! */
		InputStreamPeek* is;

		/** used to detect \r\n */
		int lastChar;

	};

}

#endif /* LINEINPUTSTREAM_H_ */
