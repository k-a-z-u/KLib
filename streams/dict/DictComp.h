#ifndef K_STREAMS_DICT_DICT_H
#define K_STREAMS_DICT_DICT_H

#include <unordered_map>
#include <string>
#include "DictHelper.h"

namespace K {

	/**
	 * the dictionary for compressing input streams
	 */
	class DictComp {

	private:

		/** the map for word->key */
		std::unordered_map<std::string, int> map;

		/** the next index to use */
		uint32_t next;

	public:

		/** ctor */
		DictComp() {
			map.reserve(DictHelper::dictSize * 2);
			for (unsigned int i = 0; i < 256; ++i) {
				std::string s = "";
				s += (uint8_t) i;
				map[s] = i;
			}
			next = 255;
		}

		/** does the dictionary contain this word? */
		bool contains(const std::string& word) {
			return map.find(word) != map.end();
		}

		/** add the given word to the dictionary */
		void add(const std::string& word) {

			// limit the dictionary's size
			if (next > DictHelper::dictSize) {return;}
			//std::cout << next << std::endl;
			map[word] = ++next;

		}

		/** get the index for the given word */
		uint32_t get(const std::string& word) {
			return map[word];
		}

	};


}

#endif // K_STREAMS_DICT_DICT_H
