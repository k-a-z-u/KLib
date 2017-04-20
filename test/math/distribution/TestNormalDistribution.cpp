

#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/distribution/Normal.h"
#include "../../../math/distribution/NormalN.h"


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

	TEST(Distribution, normal3) {

		NormalDistribution nd(0, std::sqrt(0.2));
		double d1 = nd.getProbability( 0);
		double d2 = nd.getProbability(+1);
		double d3 = nd.getProbability(-1);

		ASSERT_EQ(d2,d3);
		ASSERT_NEAR(0.8920620580763857, d1, 0.00001);
		ASSERT_NEAR(0.07322491280963242, d2, 0.00001);
		ASSERT_NEAR(0.07322491280963242, d3, 0.00001);

	}

	TEST(Distribution, normal4) {

		NormalDistribution nd(0, 2);
		double d1 = nd.getProbability( 0);
		double d2 = nd.getProbability(+1);
		double d3 = nd.getProbability(-1);

		ASSERT_EQ(d2,d3);
		ASSERT_NEAR(0.19947114020071635, d1, 0.00001);

	}

	TEST(Distribution, normalCDF1) {

		NormalDistributionCDF nd(0,1);
		ASSERT_NEAR(0.5, nd.getProbability(0), 0.00001);
		ASSERT_NEAR(0.5, NormalDistributionCDF::getProbability(0, 1, 0), 0.00001);

		ASSERT_NEAR(1.0, nd.getProbability(5), 0.00001);
		ASSERT_NEAR(1.0, NormalDistributionCDF::getProbability(0, 1, 5), 0.00001);

		ASSERT_NEAR(0.0, nd.getProbability(-5), 0.00001);
		ASSERT_NEAR(0.0, NormalDistributionCDF::getProbability(0, 1, -5), 0.00001);

	}

	TEST(Distribution, normalCDF2) {

		NormalDistributionCDF nd(3,1);
		ASSERT_NEAR(0.5, nd.getProbability(3), 0.00001);
		ASSERT_NEAR(0.5, NormalDistributionCDF::getProbability(3, 1, 3), 0.00001);

		ASSERT_NEAR(1.0, nd.getProbability(3+5), 0.00001);
		ASSERT_NEAR(1.0, NormalDistributionCDF::getProbability(3, 1, 3+5), 0.00001);

		ASSERT_NEAR(0.0, nd.getProbability(3-5), 0.00001);
		ASSERT_NEAR(0.0, NormalDistributionCDF::getProbability(3, 1, 3-5), 0.00001);

	}

	TEST(Distribution, normalN1) {

		Eigen::Vector2d val; val << 0,0;
		Eigen::Vector2d mu; mu << 0,0;
		Eigen::Matrix2d sigma; sigma << 1, 0, 0, 1;

		// 2D distribution is the same probability as 1D*1D
		NormalDistributionN<double> dist(mu, sigma);
		const double d1 = dist.getProbability(val);
		const double d0 = std::pow(NormalDistribution::getProbability(0, 1, 0), 2);
		ASSERT_NEAR(d0, d1, 0.0001);

	}

	TEST(Distribution, normalN2) {

		Eigen::Vector2d val; val << 2,4;
		Eigen::Vector2d mu; mu << 2,4;
		Eigen::Matrix2d sigma; sigma << 1, 0, 0, 1;

		// 2D distribution is the same probability as 1D*1D
		NormalDistributionN<double> dist(mu, sigma);
		const double d1 = dist.getProbability(val);
		const double d0 = std::pow(NormalDistribution::getProbability(0, 1, 0), 2);
		ASSERT_NEAR(d0, d1, 0.0001);

	}

	TEST(Distribution, normalN3) {

		Eigen::Vector2d val; val << 0,0;
		Eigen::Vector2d mu; mu << 0,0;
		Eigen::Matrix2d sigma; sigma << 2, 0, 0, 2;

		// 2D distribution is the same probability as 1D*1D
		NormalDistributionN<double> dist(mu, sigma);
		const double d1 = dist.getProbability(val);
		const double d0 = std::pow(NormalDistribution::getProbability(0, 2, 0), 2);
		ASSERT_NEAR(d0, d1, 0.0001);

		int i = 0; (void) i;

	}

}


#endif
