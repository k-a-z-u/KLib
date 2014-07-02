#ifndef TRIEINPUTSTREAM_H
#define TRIEINPUTSTREAM_H

#include "../../streams/InputStream.h"
#include <vector>

namespace K {



	class TrieInputStream : public InputStream {

	private:

		InputStream* is;

		std::vector<uint8_t> last;
		std::vector<uint8_t> cur;

		std::vector<std::vector<uint8_t>> map;

	public:

		/** ctor */
		TrieInputStream(InputStream* is) : is(is) {

			map.resize(256);
			for (uint32_t i = 0; i < 256; ++i) {
				map[i].push_back( (uint8_t) i );
			}

		}

		int read() override {

			// ensure we have something to read
			if (!fillIfNeeded()) {return -1;}

			// send the buffer's next byte and remove it from the buffer
			int out = cur[0];
			cur.erase(cur.begin());

			// done
			return out;

		}

		int read(uint8_t* data, uint32_t len) override {

			// ensure we have something to read
			if (!fillIfNeeded()) {return -1;}

			// send the whole current word
			if (len >= cur.size()) {
				uint32_t s = cur.size();
				memcpy(data, cur.data(), s);
				cur.clear();
				return s;

			// send only a part of the current word
			} else {
				memcpy(data, cur.data(), len);
				cur.erase(cur.begin(), cur.begin() + len);
				return len;

			}

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

		/** ensure we have some bytes to send */
		bool fillIfNeeded() {

			// do we still have some bytes to send?
			if (!cur.empty()) {return true;}

			// try to get the next word by its index
			if (!getNextWord()) {return false;}

			// adjust the dictionary
			if (!last.empty()) {
				last.push_back(cur[0]);
				map.push_back(last);
			}

			// everything fine
			last = cur;
			return true;

		}

		/** read the next word from the stream */
		bool getNextWord() {

			// read the next index
			int32_t idx = DictHelper::readVarLength(is);
			if (idx == -1) {return false;}

			// very rare corner-case where idx is not contained
			// (when the input contains a pattern several times)
			if (map.size() <= idx) {
//				std::cout << "FAIL" << std::endl;
//				throw "";
				last.push_back(last[0]);
				map.push_back(last);
				last.clear();
			}

			// fetch corresponding word from dict
			cur = map[idx];

			// everything fine
			return true;

		}

	};
}

#endif // TRIEINPUTSTREAM_H
