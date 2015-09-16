#ifndef K_STREAMS_WINDOW_WINDOWOUTPUTSTREAM_H
#define K_STREAMS_WINDOW_WINDOWOUTPUTSTREAM_H

#include "../../streams/OutputStream.h"
#include "WindowBuffer.h"
#include "../dict/DictHelper.h"

namespace K {

	class WindowOutputStream : public OutputStream {

	private:

		WindowBuffer buf;

		OutputStream* os;

		std::vector<uint8_t> curWord;

	public:

		/** ctor */
		WindowOutputStream(OutputStream* os) : os(os) {;}

		void write(uint8_t data) override {
			curWord.push_back(data);
			send(false);
		}

		void write(const uint8_t* data, const size_t len) override {
			for (uint32_t i = 0; i < len; ++i) {
				write(data[i]);
			}
		}

		void close() override {
			if (!curWord.empty()) {send(true);}
			buf.dump();
		}

		void flush() override {
			;
		}

	private:

		void send(bool flush) {

			WindowMatch wm = buf.getLongestMatch(curWord);
			if (wm.len < curWord.size() || flush) {

				//// add the non-matching part to the buffer
				// what to send?
				if (wm.len <= 1) {
					DictHelper::writeVarLength(os, curWord[0]);
					buf.add(curWord[0]);
					curWord.erase(curWord.begin());
					//if (wm.len == 1) {curWord.push_back(curWord.size()-1);}

				} else {
					DictHelper::writeVarLength(os, wm.idx + 256);
					//DictHelper::writeVarLength(os, wm.len);
					os->write(wm.len);

					buf.add(curWord, 0, curWord.size() - 1); //curWord.substr(0, curWord.length() - 1));
					//curWord = curWord[curWord.length() - 1];
					uint8_t last = curWord[curWord.size() - 1];
					curWord.clear();
					curWord.push_back(last);


				}

				// append the word to the end of the buffer
				//buf.add(buf.get(wm.idx, wm.len));

				// start with the last character
				//curWord = curWord[curWord.length() - 1];

			}

		}

	};

}

#endif // K_STREAMS_WINDOW_WINDOWOUTPUTSTREAM_H
