#ifndef TRIE_H
#define TRIE_H

namespace K {

	class TrieLevel;

	class TrieLevelFactory {

	private:

		std::vector<TrieLevel> entries;

	public:

		TrieLevel* get(uint8_t val, uint32_t idx);

	};


	class TrieLevel {

	public:

		uint8_t val;
		uint32_t idx;
		//std::vector<TrieLevel> childs;
		TrieLevel* firstChild;
		TrieLevel* nextSibling;

	private:

		friend class TrieLevelFactory;

		/** ctor */
		TrieLevel() : val(0), idx(0xFFFFFFFF), firstChild(nullptr), nextSibling(nullptr) {
			;
		}

		/** ctor */
		TrieLevel(uint8_t val, uint32_t idx) : val(val), idx(idx), firstChild(nullptr), nextSibling(nullptr) {
			;
		}

	public:

		/** does this level contain a child named 'val' ? */
		TrieLevel* find(uint8_t val) {
//			for (TrieLevel& tl : childs) {
//				if (tl.val == val) {return &tl;}
//			}
			TrieLevel* tl = firstChild;
			while(tl) {
				if (tl->val == val) {return tl;}
				tl = tl->nextSibling;
			}
			return nullptr;
		}

		/** append the given child */
		void add(TrieLevelFactory& factory, uint8_t val, uint32_t idx) {
			//childs.push_back(TrieLevel(val, idx));

			if (this->firstChild == nullptr) {
				this->firstChild = factory.get(val,idx);
			} else {
				TrieLevel* child = this->firstChild;
				while (child->nextSibling) {child = child->nextSibling;}
				child->nextSibling = factory.get(val, idx);
			}

		}

	};


	/** impl */
	TrieLevel* TrieLevelFactory::get(uint8_t val, uint32_t idx) {
		entries.push_back(TrieLevel(val, idx));
		TrieLevel* arr = entries.data();
		return &arr[entries.size() - 1];
	}


}

#endif // TRIE_H
