

#ifdef WITH_TESTS

#include "../Test.h"
#include "../../cv/KernelFactory.h"
#include <sstream>
using namespace K;

TEST(Kernel, Gauss2D) {

	Kernel k = KernelFactory::gauss2D(1.0);
	ASSERT_EQ(k.getWidth(), k.getHeight());
	ASSERT_NEAR(1.0, k.getSum(), 0.0001);
	ASSERT_NEAR(0.162103, k.get(2,2), 0.001);

}

TEST(Kernel, Gauss1D) {

	Kernel k = KernelFactory::gauss1D(1.0);
	ASSERT_EQ(1, k.getHeight());
	ASSERT_NEAR(1.0, k.getSum(), 0.0001);

}

#endif

