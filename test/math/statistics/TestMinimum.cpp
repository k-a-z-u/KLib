#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/statistics/Minimum.h"

namespace K {

	TEST(Minimum, calculate) {

		Minimum<float> min;

		min.add(+999);
		ASSERT_EQ(999, min.get());

		min.add(1);
		ASSERT_EQ(1, min.get());

		min.add(2);
		ASSERT_EQ(1, min.get());

		min.add(0);
		ASSERT_EQ(0, min.get());

		min.add(-99);
		ASSERT_EQ(-99, min.get());

	}

}

#endif
