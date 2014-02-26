/*
 * TestFixedPool.h
 *
 *  Created on: Jan 21, 2014
 *      Author: kazu
 */



#ifdef WITH_TESTS
#include "../Test.h"
#include "../TestHelper.h"
#include "../../memory/FixedPool.h"

namespace K {

	TEST(FixedPool, Create) {

		// the struct to store
		struct Dummy {
			uint8_t intVal;
			float floatVal;
		};

		K::FixedPool<Dummy> pool;

		ASSERT_EQ(nullptr, pool.freeList);
		ASSERT_EQ(nullptr, pool.chunkList);

		// allocate max-size minus 1 elements
		for (unsigned int i = 0; i < (K::FixedPool<Dummy>::Chunk::numEntries-1); ++i) {
			pool.alloc();
			ASSERT_NE(nullptr, pool.freeList);
		}

		// after allocating another element, the current chunk should be filled completely
		Dummy* m = pool.alloc();
		ASSERT_EQ(nullptr, pool.freeList);
		ASSERT_NE(nullptr, pool.chunkList);

		// one chunk should be freed by now
		pool.free(m);
		ASSERT_NE(nullptr, pool.freeList);

		Dummy* m2 = pool.alloc();
		ASSERT_EQ(nullptr, pool.freeList);
		ASSERT_EQ(m, m2);

	}

}

#endif
