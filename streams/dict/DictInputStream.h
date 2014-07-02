#ifndef K_STREAMS_DICT_DICTINPUTSTREAM_H
#define K_STREAMS_DICT_DICTINPUTSTREAM_H

#include "DictDecomp.h"
#include "DictHelper.h"
#include "../InputStream.h"

namespace K {

	class DictInputStream : public InputStream {

	private:

		InputStream* is;

		DictDecomp dictDecomp;

		std::string lastEntry;
		std::string readBuf;

	public:

		/** ctor */
		DictInputStream(InputStream* is) : is(is) {

		}

		int read() {

			// does the buffer contain some bytes to send?
			if (readBuf.empty()) {

				// try to get the next word by its index
				if (!getNextWord()) {return -1;}

				if (!lastEntry.empty()) {
					std::string next = lastEntry + readBuf[0];
					std::cout << "new: " << next << std::endl;
					dictDecomp.add(next);
				}

				lastEntry = readBuf;

			}

			// send the buffer's next byte and remove it from the buffer
			int out = readBuf[0];
			readBuf = readBuf.substr(1);

			return out;

		}

		void skip(uint64_t n) override {
			;
			// TODO
			;
		}

		void close() override {
			is->close();
		}

	private:

		/** read the next index from the underlying input stream */
		int32_t readIdx() {
			return DictHelper::readVarLength(is);
		}

		/** read the next word from the stream */
		bool getNextWord() {

			// read the next index
			int32_t idx = readIdx();
			if (idx == -1) {return false;}

			// sanity check
			if (!dictDecomp.contains(idx)) {
				std::cout << "index not found: " << idx << std::endl;
				throw "";
			}

			// fetch corresponding word from dict
			readBuf = dictDecomp.get(idx);

			return true;

		}

	};

}

#endif // K_STREAMS_DICT_DICTINPUTSTREAM_H
