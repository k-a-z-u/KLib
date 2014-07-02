#ifndef K_STREAMS_WINDOW_WINDOWBUFFER_H
#define K_STREAMS_WINDOW_WINDOWBUFFER_H

#include <list>

#include <algorithm>
#include <iostream>
#include "../StreamException.h"

namespace K {

	struct WindowMatch {
		uint32_t idx;
		uint32_t len;
		WindowMatch() : idx(0), len(0) {;}
	};

	class WindowBuffer {


	private:

		unsigned int size;

		unsigned int head;

		uint8_t* buf;

		std::list<uint32_t> index[256];

		int debug = 1;

	public:

		/** ctor */
		WindowBuffer() : size(1024*1024), head(0) {

			buf = new uint8_t[size];

			// fill the buffer with each byte
			// this makes things a lot easier within the rest of the code!
			//for (unsigned int i = 0; i < 256; ++i) { add(i); }

		}

		/** dtor */
		~WindowBuffer() {
			delete[] buf;
		}

		/** add the given word to the buffer */
		void add(const std::vector<uint8_t>& word, uint32_t start, uint32_t len) {
			for (uint32_t i = 0; i < len; ++i) {
				add(word[start+i]);
			}
		}

		/** append the given byte at the end of the buffer */
		void add(uint8_t byte) {

			const uint32_t maxEntries = 128;

			remove(head);
			if (debug >= 2) {std::cout << "\tadding " << (int) byte << " to index " << head << std::endl;}
			buf[head] = byte;
			if (index[byte].size() > maxEntries) {
			//	index[byte].pop_back();
			}
			index[byte].push_back(head);
			head = (head + 1) % size;
		}

		void remove(uint32_t idx) {
			uint8_t byte = buf[idx];
			if (debug >= 2) {std::cout << "\tremoving " << (int) byte << " at index " << idx << std::endl;}
			//std::cout << (unsigned int) byte << std::endl;
			//std::cout << index[byte].size() << std::endl;
			std::list<uint32_t>& lst = index[byte];
			lst.erase( std::remove(lst.begin(), lst.end(), idx), lst.end() );
			//std::cout << index[byte].size() << std::endl;
			//std::cout << "-----" << std::endl;
		}

		/** get the word starting at the given index */
		std::vector<uint8_t> get(uint32_t idx, uint32_t len) const {
			if (len >= size) {throw StreamException("invalid length given!");}
			if (len == 0) {
				throw StreamException("length must not be zero!");
			}
			std::vector<uint8_t> vec;
			while(len--) {
				vec.push_back(buf[idx]);
				idx = (idx + 1) % size;
			};
			return vec;
		}

		/** find the longest match for the given word */
		WindowMatch getLongestMatch(const std::vector<uint8_t>& word) const {

			//if (debug >= 2) {std::cout << "longest match for '" << word << "'" << std::endl;}

			const uint32_t maxMatchLen = 255;
			WindowMatch bestMatch;

			for (uint32_t idx : index[word[0]]) {

				uint32_t i = 1;
				while(
					  buf[(idx+i)] == word[0+i] &&	// chars still match
					  i < word.size() &&			// word not done
					  i < maxMatchLen &&			// custom limit
					  idx+i < size
				) {++i;}

				//if (debug >= 2) {std::cout << "\tidx: " << idx << " len: " << i << std::endl;}

				// have we found a better match?
				if (i > bestMatch.len) {
					bestMatch.len = i;
					bestMatch.idx = idx;
				}

				// it will be very time consuming to find a longer match
				if (i == maxMatchLen) {break;}

			}

			// done
			if (debug >= 2) {std::cout << "\t-> idx: " << bestMatch.idx << " len: " << bestMatch.len << std::endl;}
			return bestMatch;

		}

		void dump() {
			std::string s((char*)(buf+0), 64);
			std::cout << "---------------------------------" << std::endl;
			std::cout << s << std::endl;
			std::cout << std::endl << std::endl << std::endl;
		}

		void dumpStr(const std::string& str) {
			std::cout << "'" << str << "' -> ";
			for (unsigned int i = 0; i < str.length(); ++i) {
				std::cout << (int) (uint8_t) str[i] << ' ';
			}
			std::cout << std::endl;
		}





	};

}

#endif // K_STREAMS_WINDOW_WINDOWBUFFER_H
