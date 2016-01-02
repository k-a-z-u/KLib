
/*
 * TestNumOptGradient.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include <omp.h>

#include "../../../math/neuralnet/FeedForwardNeuralNet.h"
namespace K {

	TEST(FeedForwardNeuralNet, configure) {

		K::FeedForwardNeuralNet<float, K::FeedForwardNeuralNetOPKeep> net;
		net.setLayers({60, 16, 4});
		ASSERT_EQ(3, net.getNumLayers());
		ASSERT_EQ(60, net.getLayerSize(0));
		ASSERT_EQ(16, net.getLayerSize(1));
		ASSERT_EQ(4, net.getLayerSize(2));
		ASSERT_EQ(60*16 +16*4, net.getNumFactors());
		ASSERT_EQ(60+16+4, net.getNumTemporals());

	}

	TEST(FeedForwardNeuralNet, factorize) {

		K::FeedForwardNeuralNet<float, K::FeedForwardNeuralNetOPKeep> net;
		net.setLayers({4, 3, 2, 1});
		ASSERT_EQ(4, net.getNumLayers());
		ASSERT_EQ(4, net.getLayerSize(0));
		ASSERT_EQ(3, net.getLayerSize(1));
		ASSERT_EQ(2, net.getLayerSize(2));
		ASSERT_EQ(1, net.getLayerSize(3));
		ASSERT_EQ(4*3 +3*2 +2*1, net.getNumFactors());
		ASSERT_EQ(4+3+2+1, net.getNumTemporals());

		net.setFactors({1,1,1,1, 2,3,4,5, 1,2,3,4,   1,2,3,  5,4,3,    7,9});
		std::vector<float> res = net.get({2,4,6,8});

		ASSERT_EQ(2, net.temporals[0]);
		ASSERT_EQ(4, net.temporals[1]);
		ASSERT_EQ(6, net.temporals[2]);
		ASSERT_EQ(8, net.temporals[3]);

		ASSERT_EQ(2*1 +4*1 +6*1 +8*1, net.temporals[4]);
		ASSERT_EQ(2*2 +3*4 +4*6 +5*8, net.temporals[5]);
		ASSERT_EQ(1*2 +2*4 +3*6 +4*8, net.temporals[6]);

		ASSERT_EQ(1*net.temporals[4] + 2*net.temporals[5] + 3*net.temporals[6], net.temporals[7]);
		ASSERT_EQ(5*net.temporals[4] + 4*net.temporals[5] + 3*net.temporals[6], net.temporals[8]);

		ASSERT_EQ(7*net.temporals[7] + 9*net.temporals[8], net.temporals[9]);

		ASSERT_EQ(1, res.size());
		ASSERT_EQ(net.temporals[9], res[0]);


	}

	TEST(FeedForwardNeuralNet, logistic) {

		K::FeedForwardNeuralNet<float, K::FeedForwardNeuralNetOPLogistic> net;


	}

	TEST(FeedForwardNeuralNet, duty) {

		K::FeedForwardNeuralNet<float, K::FeedForwardNeuralNetOPKeep> net;
		net.setLayers({512, 32, 8, 2});
		std::vector<float> val; val.resize(512);

		omp_set_dynamic(false);
		omp_set_num_threads(2);
		for (int i = 0; i < 10240; ++i) {
			net.get(val, true);
		}

	}

}

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
