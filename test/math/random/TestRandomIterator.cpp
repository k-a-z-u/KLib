
#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/random/RandomIterator.h"


namespace K {

	TEST(RandomIterator, init) {

		int cnt;
		std::vector<int> values = {1,2,3,4,5,6};

		RandomIterator<int> it1(values, 1); it1.randomize();
		cnt = 0; for(int i : it1) {(void) i; ++cnt;} ASSERT_EQ(1, cnt);

		RandomIterator<int> it2(values, 2); it2.randomize();
		cnt = 0; for(int i: it2) {(void) i; ++cnt;} ASSERT_EQ(2, cnt);

		RandomIterator<int> it3(values, 3); it3.randomize();
		cnt = 0; for(int i: it3) {(void) i; ++cnt;} ASSERT_EQ(3, cnt);

		RandomIterator<int> it4(values, 4); it4.randomize();
		cnt = 0; for(int i: it4) {(void) i; ++cnt;} ASSERT_EQ(4, cnt);

		RandomIterator<int> it5(values, 5); it5.randomize();
		cnt = 0; for(int i: it5) {(void) i; ++cnt;} ASSERT_EQ(5, cnt);

		RandomIterator<int> it6(values, 6); it6.randomize();
		cnt = 0; for(int i: it6) {(void) i; ++cnt;} ASSERT_EQ(6, cnt);

	}

	TEST(RandomIterator, tooMany) {

		std::vector<int> values = {1,2,3,4,5,6};
		ASSERT_THROW( RandomIterator<int>(values, 7), std::exception );

	}

	TEST(RandomIterator, none) {

		std::vector<int> values1 = {1,2,3,4,5,6};
		ASSERT_THROW( RandomIterator<int>(values1, 0), std::exception );

		std::vector<int> values2 = {};
		ASSERT_THROW( RandomIterator<int>(values2, 1), std::exception );

	}

}


#endif
