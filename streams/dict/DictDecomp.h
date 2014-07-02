#ifndef DICTDECOMP_H
#define DICTDECOMP_H

#include <unordered_map>
#include <string>
#include "DictHelper.h"

namespace K {

	class DictDecomp {

	private:

		std::unordered_map<uint32_t, std::string> map;

		uint32_t next;

	public:

		/** ctor */
		DictDecomp() {
			for (uint32_t i = 0; i < 256; ++i) {
				std::string s = "";
				s += (char) i;
				map[i] = s;
			}
			next = 255;
		}

		/** get the word for the given index */
		const std::string& get(uint32_t idx) {
			return map[idx];
		}

		/** does the map contain the given index? */
		bool contains(uint32_t idx) {
			return map.find(idx) != map.end();
		}

		/** add the given word to the next free slot */
		void add(const std::string& word) {
			if (next > DictHelper::dictSize) {return;}
			std::cout << "new idx: " << next << std::endl;
			map[++next] = word;
		}

	};

}

#endif // DICTDECOMP_H
