

#ifdef WITH_TESTS

#include "../Test.h"
#include "../../cv/KernelFactory.h"
#include "../../cv/Convolve.h"
#include <sstream>
using namespace K;

TEST(Convolve, Gauss1D) {

	Kernel k = KernelFactory::gauss1D(1.0, 3);
	ASSERT_EQ(3, k.getWidth());
	ASSERT_EQ(1, k.getHeight());

	// symmetry
	ASSERT_EQ(k.get(0,0), k.get(2,0));

	// unit-impulse
	ImageChannel img(3,1);
	img.set(1,0,1);

	// image = kernel (due to unit impulse)
	ImageChannel img2 = Convolve::run(img, k);
	ASSERT_EQ(img2, k);

}

TEST(Convolve, Gauss2D) {

	Kernel k = KernelFactory::gauss2D(1.0, 3);
	ASSERT_EQ(3, k.getWidth());
	ASSERT_EQ(3, k.getHeight());

	// symmetry
	ASSERT_EQ(k.get(0,0), k.get(2,0));
	ASSERT_EQ(k.get(0,1), k.get(2,1));
	ASSERT_EQ(k.get(0,2), k.get(2,2));
	ASSERT_EQ(k.get(1,0), k.get(1,2));

	// unit-impulse
	ImageChannel img(3,3);
	img.set(1,1,1);

	// image = kernel (due to unit impulse)
	ImageChannel img2 = Convolve::run(img, k);
	ASSERT_EQ(img2, k);

}

#endif


