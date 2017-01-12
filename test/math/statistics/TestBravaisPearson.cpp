#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/statistics/BravaisPearsonCorrelation.h"

using namespace K;

TEST(BravaisPearson, eq) {

	std::vector<uint8_t> a = {1, 2, 3, 4, 5, 6, 7};
	std::vector<uint8_t> b = {1, 2, 3, 4, 5, 6, 7};
	const float res = BravaisPearsonCorrelation::get(a, b);

	ASSERT_NEAR(1.0, res, 0.0001);

}

TEST(BravaisPearson, prop) {

	std::vector<uint8_t> a = {1,   2,   3,   4,   5,   6,   7};
	std::vector<uint8_t> b = {1*2, 2*2, 3*2, 4*2, 5*2, 6*2, 7*2};
	const float res = BravaisPearsonCorrelation::get(a, b);

	ASSERT_NEAR(1.0, res, 0.0001);

}

TEST(BravaisPearson, mirror) {

	std::vector<uint8_t> a = {1, 2, 3, 4, 5, 6, 7};
	std::vector<uint8_t> b = {7, 6, 5, 4, 3, 2, 1};
	const float res = BravaisPearsonCorrelation::get(a, b);

	ASSERT_NEAR(-1.0, res, 0.0001);

}

#endif
