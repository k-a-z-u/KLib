
/*
 * TestNumOptGradient.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"
/*
#include "../../../math/neuralnet/NeuralNetIHO.h"

namespace K {

	TEST(NeuralNet, IHO1) {

		NeuralNetIHO<5, 3, 1> net;

		float a[] = {0,0,0,0,0};
		float b[] = {0,0,0,0,0};
		float c[] = {0,0,0,0,0};
		float d[] = {0,0,0};
		float e[] = {1,2,3,4,5};

		net.setInToHiddenWeights(0, a);
		net.setInToHiddenWeights(1, b);
		net.setInToHiddenWeights(2, c);

		net.setHiddenToOutputWeights(0, d);

		NeuralNetResult<1> res = net.getOutput(e);

		ASSERT_EQ(0.5, res.values[0]);

	}

	TEST(NeuralNet, IHO2) {

		NeuralNetIHO<5, 3, 1> net;

		float a[] = {-1,-1,-1,-1,-1};
		float b[] = {-1,-1,-1,-1,-1};
		float c[] = {-1,-1,-1,-1,-1};
		float d[] = {1,1,1};
		float e[] = {1,2,3,4,5};

		net.setInToHiddenWeights(0, a);
		net.setInToHiddenWeights(1, b);
		net.setInToHiddenWeights(2, c);

		net.setHiddenToOutputWeights(0, d);

		NeuralNetResult<1> res = net.getOutput(e);

		ASSERT_EQ(0.5, res.values[0]);

	}

	TEST(NeuralNet, IHO3) {

		NeuralNetIHO<5, 3, 1> net;

		float a[] = {100,100,100,100,100};
		float b[] = {100,100,100,100,100};
		float c[] = {100,100,100,100,100};
		float d[] = {100,100,100,100,100};
		float e[] = {1,2,3,4,5};

		net.setInToHiddenWeights(0, a);
		net.setInToHiddenWeights(1, b);
		net.setInToHiddenWeights(2, c);

		net.setHiddenToOutputWeights(0, d);

		NeuralNetResult<1> res = net.getOutput(e);

		ASSERT_EQ(1.0, res.values[0]);

	}

}

*/

#endif
