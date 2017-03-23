#ifndef TESTLINEARTRANSFORM_CPP
#define TESTLINEARTRANSFORM_CPP


#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/linearTransform/GeneticLinearTransform.h"

#include <functional>

namespace K {

	TEST(GeneticLinearTransform, test1) {

		GeneticLinearTransform<float, int> glt;

		glt.addTrainingPattern(0, {+0, -1, -0});
		glt.addTrainingPattern(0, {+2, -3, -1});
		glt.addTrainingPattern(0, {+0, -2, -0});

		glt.addTrainingPattern(1, {+0, +3, -0});
		glt.addTrainingPattern(1, {+2, +5, -1});
		glt.addTrainingPattern(1, {+0, +4, -0});

//		glt.usedOnlyBestValues(GeneticLinearTransform<float, int>::maximizeDistance, 1);
		glt.estimate(GeneticLinearTransform<float, int>::maximizeDistance);

		int i = 0; (void) i;

	}

	TEST(GeneticLinearTransform, testLines) {

		GeneticLinearTransform<float, int> glt;

		// class1: diagonal line
		for (int i = -50; i < +50; ++i) {
			const float x = i;
			const float y = i;
			glt.addTrainingPattern(0, {x, y});
		}

		// class2: diagonal line shifted to the right
		for (int i = -50; i < 50; ++i) {
			const float x = i + 10;
			const float y = i;
			glt.addTrainingPattern(1, {x, y});
		}

		glt.estimate(GeneticLinearTransform<float, int>::maximizeDistance);
		//glt.debugView();

		// mat[0] apx -mat[1]	// 45 degree rotation
		ASSERT_NEAR(glt.getMatrix()[0], -glt.getMatrix()[1], 0.001);

		int i = 0; (void) i;

	}

}

#endif


#endif
