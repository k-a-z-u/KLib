#ifndef NEURALNETIHO_H
#define NEURALNETIHO_H

#include "../../Assertions.h"
#include <vector>

namespace K {

	template <int numOut> struct NeuralNetResultIHO {
		float values[numOut];
	};


	template <int numIn, int numHidden, int numOut> class NeuralNetIHO {


	public:

		void setInToHiddenWeights(const unsigned int idxHidden, const float values[numIn]) {
			memcpy(inWeights[idxHidden], values, sizeof(float) * numIn);
		}

		void setHiddenToOutputWeights(const unsigned int idxOutput, const float values[numHidden]) {
			memcpy(hiddenWeights[idxOutput], values, sizeof(float) * numHidden);
		}

		void setAll(const std::vector<float> values) {
			_assertEqual(numIn*numHidden + numHidden*numOut, values.size(), "invalid number of values");
			setAll(values.data());
		}

		void setAll(const float* values) {
			for (int h = 0; h < numHidden; ++h) {
				setInToHiddenWeights(h, &values[h*numIn]);
			}
			for (int o = 0; o < numOut; ++o) {
				setHiddenToOutputWeights(o, &values[numIn*numHidden+o*numHidden]);
			}
		}

		/** get the networks output for the given input */
		NeuralNetResultIHO<numOut> getOutput(const float input[numIn]) {

			float hidden[numHidden];
			float out[numOut];

			memset(hidden, 0, sizeof(float)*numHidden);
			memset(out, 0, sizeof(float)*numOut);

			// calculate hidden layer
			for (unsigned int idxHidden = 0; idxHidden < numHidden; ++idxHidden) {
				for (unsigned int idxIn = 0; idxIn < numIn; ++idxIn) {
					hidden[idxHidden] += input[idxIn] * inWeights[idxHidden][idxIn];
				}
				hidden[idxHidden] = 1.0f / (1.0f + std::exp(-hidden[idxHidden]));
			}

			// calculate output layer
			for (unsigned int idxOut = 0; idxOut < numOut; ++idxOut) {
				for (unsigned int idxHidden = 0; idxHidden < numHidden; ++idxHidden) {
					out[idxOut] += hidden[idxHidden] * hiddenWeights[idxOut][idxHidden];
				}
				out[idxOut] = 1.0f / (1.0f + std::exp(-out[idxOut]));
			}

			NeuralNetResultIHO<numOut> res;
			memcpy(res.values, out, sizeof(float)*numOut);
			return res;

		}


	private:

		float inWeights[numHidden][numIn];
		float hiddenWeights[numOut][numHidden];


	};

}

#endif // NEURALNETIHO_H
