#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/statistics/Median.h"

namespace K {

	TEST(Median, calculate) {

		Median<float> med;

		ASSERT_THROW(med.get(), std::exception);

		med.add(1);
		ASSERT_EQ(1, med.get());

		med.add(2);
		ASSERT_EQ(1.5, med.get());

		med.add(3);
		ASSERT_EQ(2, med.get());

		med.add(99);
		ASSERT_EQ(2.5, med.get());


	}

}

#endif
