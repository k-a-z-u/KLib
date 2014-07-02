#ifndef NEURALNETIHO_H
#define NEURALNETIHO_H

namespace K {

	template <int numOut> struct NeuralNetResultIHO {
		float values[numOut];
	};


	template <int numIn, int numHidden, int numOut> class NeuralNetIHO {


	public:

		void setInToHiddenWeights(unsigned int idxHidden, float values[numIn]) {
			memcpy(inWeights[idxHidden], values, sizeof(float) * numIn);
		}
		void setHiddenToOutputWeights(unsigned int idxOutput, float values[numHidden]) {
			memcpy(hiddenWeights[idxOutput], values, sizeof(float) * numHidden);
		}

		/** get the networks output for the given input */
		NeuralNetResult<numOut> getOutput(float input[numIn]) {

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

			NeuralNetResult<numOut> res;
			memcpy(res.values, out, sizeof(float)*numOut);
			return res;

		}


	private:

		float inWeights[numHidden][numIn];
		float hiddenWeights[numOut][numHidden];


	};

}

#endif // NEURALNETIHO_H
