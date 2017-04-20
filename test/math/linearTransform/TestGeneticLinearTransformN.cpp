#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/linearTransform/GeneticLinearTransformN.h"

#include <functional>

namespace K {

	TEST(GeneticLinearTransformN, test1) {

		std::minstd_rand gen;
		std::uniform_real_distribution<float> dNoise(-0.4, +0.4);

		GeneticLinearTransformN<float, int> glt(2);

		for (int c = 0; c < 4; ++c) {
			for (int i = -0; i < 200; ++i) {
				const float x = (float)c + dNoise(gen);
				const float y = (float)i + dNoise(gen);
				const float z = (float)c * i + dNoise(gen);
				glt.addTrainingPattern(c, (std::vector<float>){x,y,z});
			}
		}

		glt.estimate(glt.mahalanobis);

		sleep(100);

	}


}

#endif
