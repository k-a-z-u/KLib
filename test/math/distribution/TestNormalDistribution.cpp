

#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/distribution/Normal.h"


namespace K {


	TEST(Distribution, normal1) {

		double d1 = NormalDistribution::getProbability(0, 1,  0);
		double d2 = NormalDistribution::getProbability(0, 1, +1);
		double d3 = NormalDistribution::getProbability(0, 1, -1);

		ASSERT_EQ(d2,d3);
		ASSERT_NEAR(0.398942, d1, 0.00001);
		ASSERT_NEAR(0.241971, d2, 0.00001);
		ASSERT_NEAR(0.241971, d3, 0.00001);

	}

	TEST(Distribution, normal2) {

		NormalDistribution nd(0,1);
		double d1 = nd.getProbability( 0);
		double d2 = nd.getProbability(+1);
		double d3 = nd.getProbability(-1);

		ASSERT_EQ(d2,d3);
		ASSERT_NEAR(0.398942, d1, 0.00001);
		ASSERT_NEAR(0.241971, d2, 0.00001);
		ASSERT_NEAR(0.241971, d3, 0.00001);

	}

}


#endif
