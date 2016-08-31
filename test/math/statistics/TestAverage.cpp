#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/statistics/Average.h"

namespace K {

	TEST(Average, calculate) {

		Average<float> avg;

		ASSERT_THROW(avg.get(), std::exception);

		avg.add(1);
		ASSERT_EQ(1, avg.get());

		avg.add(2);
		ASSERT_EQ(1.5, avg.get());

		avg.add(3);
		ASSERT_EQ(2, avg.get());

		avg.add(99);
		ASSERT_EQ(26.25, avg.get());

	}

}

#endif
