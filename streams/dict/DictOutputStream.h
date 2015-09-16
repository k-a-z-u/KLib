#ifndef K_STREAMS_DICTOUTPUTSTREAM_H
#define K_STREAMS_DICTOUTPUTSTREAM_H

#include "../OutputStream.h"
#include "DictComp.h"
#include "DictHelper.h"

#include <string>

namespace K {

	/**
	 * output-stream using a very simple dictionary compression
	 * like LZW
	 */
	class DictOutputStream : public OutputStream {

	private:

		OutputStream* os;

		std::string cur;
		std::string prev;

		DictComp dict;

	public:

		/** ctor */
		DictOutputStream(OutputStream* os) : os(os) {;}

		/** append the given byte */
		void write(const uint8_t byte) {

			//prev = cur;
			cur += (char) byte;

			if (!dict.contains(cur)) {

				//std::cout << "missing: '" << buf << "'" << std::endl;
				std::string prev = cur.substr(0, cur.length() - 1);
				send(prev);

				dict.add(cur);
				cur = (char) byte;


			}// else {
			//	std::cout << "contains: " << buf << std::endl;
			//}

			///std::cout << "--------" << std::endl;

		}

		void write(const uint8_t* data, const size_t len) override {
			for (unsigned int i = 0; i < len; ++i) {
				write(data[i]);
			}
		}

		void close() {
			send(cur);
			cur = "";
		}

		void flush() {
			;
		}

	private:

		/** send the index of the given word to the output stream */
		void send(const std::string& word) {

			// the index to write
			uint32_t idx = dict.get(word);
			//std::cout << "out: " << word << " idx: " << idx << std::endl;

			// variable length coding
			DictHelper::writeVarLength(os, idx);

		}

	};

}

#endif // K_STREAMS_DICTOUTPUTSTREAM_H
