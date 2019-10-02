#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/optimization/NumOptAlgoPSO.h"

TEST(NumericalOptimizationPSO, testSimple) {

	auto funcRosenbrock = [] (const float* data) -> float {
		return pow(1 - data[0], 2) + 100 * pow( (data[1] - data[0]*data[0]), 2);
	};

	auto callback = [] (const int run, const float res, const float* values) {
		std::cout << "run " << run << " res: " << res << " x1: " << values[0] << " x2: " << values[1] << std::endl;
	};

	K::NumOptAlgoPSO<float> opt(2);
	std::vector<float> res; res.resize(2);
	opt.setCallback(callback);
	opt.calculateMinimum(funcRosenbrock, res.data());

}

#endif
