#ifndef TRIESTREAM_H
#define TRIESTREAM_H

#include "../../streams/OutputStream.h"
#include "../dict/DictHelper.h"
#include <vector>
#include "Trie.h"

namespace K {


	class TrieStream : public OutputStream {

	private:

		OutputStream* os;

		TrieLevelFactory factory;

		TrieLevel* root;

		TrieLevel* cur;

		uint32_t next = 0;

	public:

		TrieStream(OutputStream* os) : os(os) {

			root = factory.get(0,0xFFFFFFFF);
			cur = root;

			for (uint32_t i = 0; i < 256; ++i) {
				//TrieLevel tl(i, next++);
				root->add(factory, i, next++);
			}
		}

		void write(const uint8_t *data, unsigned int len) {
			for (uint32_t i = 0; i < len; ++i) {
				write(data[i]);
			}
		}

		void write(uint8_t data) {

			TrieLevel* n = cur->find(data);
			if (n == nullptr) {
				//std::cout << "    add: " << (char) data << std::endl;
				cur->add(factory, data, next++);
				//std::cout << "    *: " << cur->idx << std::endl;
				DictHelper::writeVarLength(os, cur->idx);
				//std::cout << "  starting: " << (char) data << std::endl;
				cur=root->find(data);

			} else {
				//std::cout << "found: " << (char) data << std::endl;
				cur = n;

			}

		}

		void close() {
			DictHelper::writeVarLength(os, cur->idx);
		}

		void flush() {

		}

	};


}

#endif // TRIESTREAM_H
