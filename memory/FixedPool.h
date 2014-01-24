#ifndef FIXEDPOOL_H
#define FIXEDPOOL_H

#include <cstdlib>

namespace K {

	/**
	 * fast allocation for many small, fixed-size elements
	 */
	template <typename T> class FixedPool {

	private:

		/** on entry which can be alloc-ed / free-ed */
		struct Entry {

			/** pointer to next element */
			Entry* next;

			/** the element itself */
			T element;

		};

		/** one chunk of memory */
		struct Chunk {

			/** the number of bytes to allocate at once */
			static const unsigned int numEntries = 4096;

			/** pointer to next chnunk */
			Chunk* next;

			/** the allocated memory */
			Entry entries[numEntries];

		};

	public:

		/** ctor */
		FixedPool() : freeList(nullptr), chunkList(nullptr) {
			;
		}

		/** dtor */
		~FixedPool() {

			// delete all chunks
			Chunk* p = chunkList;
			while(p) {
				Chunk* n = p->next;
				delete p;
				p = n;
			}

		}

		/** allocate a new element */
		T* alloc() {

			// check whether free items exist
			if (!freeList) {grow();}

			// get space for new entry and remove from the free-list
			Entry* p = freeList;
			freeList = p->next;

			return &(p->element);

		}

		/** free the given element */
		void free(T* elem) {

			// cast to entry to access the next pointer
			Entry* p = (Entry*) (((uint8_t*) elem) - sizeof(Entry*));

			// add back to free list
			p->next = freeList;
			freeList = p;

		}

	private:

		/** grow the buffer, allocate new memory */
		void grow() {

			// allocate new memory chunk
			Chunk* c = new Chunk();

			// attach to chunk-list
			c->next = chunkList;
			chunkList = c;

			// update free-list for entries
			for (unsigned int i = 0; i < Chunk::numEntries; ++i) {
				c->entries[i].next = freeList;
				freeList = &(c->entries[i]);
			}

		}


		/** hidden copy ctor */
		FixedPool (const FixedPool&);

		/** hidden assignment operator */
		void operator = (FixedPool&);


		/** g-test */
		friend class FixedPool_Create_Test;

		/** track a linked list of free entries here */
		Entry* freeList;

		/** track a linked list of all allocated chunks */
		Chunk* chunkList;

	};

}

#endif // FIXEDPOOL_H
