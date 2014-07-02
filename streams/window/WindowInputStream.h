#ifndef K_STREAMS_WINDOW_WINDOWINPUTSTREAM_H
#define K_STREAMS_WINDOW_WINDOWINPUTSTREAM_H

#include "WindowBuffer.h"
#include "../../streams/InputStream.h"
#include "../dict/DictHelper.h"

namespace K {

	class WindowInputStream : public InputStream {

	private:

		WindowBuffer buf;

		InputStream* is;

		//std::string curWord;
		std::vector<uint8_t> curWord;

		int debug = 0;

	public:

		/** ctor */
		WindowInputStream(InputStream* is) : is(is) {

		}

		int read() override {

			// buffer empty? -> fetch next word
			if (curWord.empty()) {
				if (!getNextWord()) {return -1;}
			}

			uint8_t out = curWord[0];
			//curWord = curWord.substr(1);
			curWord.erase(curWord.begin());

			return out;

		}

		void close() override {

			buf.dump();

		}

		void skip(const uint64_t n) override {
			// TODO
		}

	private:

		bool getNextWord() {

			//sleep(1);

			// get the index of the next entry to decode
			int idx = DictHelper::readVarLength(is);
			if (idx == -1) {return false;}

			// index < 256 -> output directly
			if (idx <= 0xFF) {
				if (debug >= 1) {std::cout << "byte: " << idx << std::endl;}
				//curWord = "";
				//curWord += (char) idx;
				//buf.add(curWord);
				curWord.clear();
				curWord.push_back(idx);
				buf.add(idx);

			} else {
				idx -= 256;
				int len = is->read();//DictHelper::readVarLength(is);
				if (len == -1) {return false;}

				if (debug >= 1) {std::cout << "idx: " << idx << " len: " << len << std::endl;}
				curWord = buf.get(idx, len);
				//if (debug >= 1) {std::cout << "buffer: " << curWord << std::endl;}

				//buf.add(curWord);
				buf.add(curWord, 0, curWord.size());

			}

			return true;

		}

	};

}

#endif // K_STREAMS_WINDOW_WINDOWINPUTSTREAM_H
