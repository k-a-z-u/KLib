#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/statistics/Maximum.h"

namespace K {

	TEST(Maximum, calculate) {

		Maximum<float> max;

		max.add(-999);
		ASSERT_EQ(-999, max.get());

		max.add(1);
		ASSERT_EQ(1, max.get());

		max.add(2);
		ASSERT_EQ(2, max.get());

		max.add(1);
		ASSERT_EQ(2, max.get());

		max.add(99);
		ASSERT_EQ(99, max.get());

	}

}

#endif
