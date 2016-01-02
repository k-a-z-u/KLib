#ifndef NEURALNET_H
#define NEURALNET_H

#include <vector>
#include "../../Assertions.h"

namespace K {


	struct FeedForwardNeuralNetOPKeep {
		template <typename T> static T get(const T val, const T factor)	{return val * factor;}
		template <typename T> static T post(const T val)				{return val;}
	};

	/** default calculation for Neural-Networks */
	struct FeedForwardNeuralNetOPLogistic {
		template <typename T> static T get(const T val, const T factor)	{return val * factor;}
		template <typename T> static T post(const T val)				{return (T)1.0 / ((T)1.0 + std::exp(val));}
	};

	/**
	 * a very simple feed-forward-only Neural-Network
	 */
	template <typename T, typename OP> class FeedForwardNeuralNet {

	public:

		friend class FeedForwardNeuralNet_factorize_Test;

		/** the factors to apply between several layers */
		std::vector<T> factors;

		/** intermediate values */
		std::vector<T> temporals;

		/** layer configuration */
		std::vector<int> layers;


	public:

		void setLayers(std::initializer_list<int> lst) {

			_assertBetween(lst.size(), 3, 8, "NeuralNet needs between 3 and 8 layers");

			// apply layer-size information
			for (size_t layer = 0; layer < lst.size(); ++layer) {
				layers.push_back(lst.begin()[layer]);
			}

			// allocate layer memory
			factors.resize(getNumFactors());
			temporals.resize(getNumTemporals());

		}

		/** get the number of layers within this network */
		int getNumLayers() const {
			return (int) layers.size();
		}

		/** get the size (number of values) of the idx-th layer */
		int getLayerSize(const int idx) const {
			return (int) layers[idx];
		}

		/** set all the factors to use for calculating the output */
		void setFactors(const std::vector<T>& factors) {
			setFactors(factors.data(), factors.size());
		}

		/** set all the factors to use for calculating the output */
		void setFactors(const T* data, const size_t num) {
			_assertEqual(getNumFactors(), num, "number of factors must be " + std::to_string(getNumFactors()));
			memcpy(factors.data(), data, num*sizeof(T));
		}

		std::vector<T> get(const std::vector<T>& input, const bool useOMP = false) {
			return get(input.data(), input.size(), useOMP);
		}

		std::vector<T> get(std::initializer_list<T> lst) {
			return get(lst.begin(), lst.size());
		}

		std::vector<T> get(const T* input, const size_t num, const bool useOMP = false) {
			_assertEqual(getLayerSize(0), num, "number of input values must be " + std::to_string(getLayerSize(0)));

			// reset all temporals to 0
			std::fill(temporals.begin(), temporals.end(), 0);

			// copy the input into the temporals
			memcpy(temporals.data(), input, num*sizeof(T));

			int _fOff = 0;
			int tOffI = 0;
			int tOffO = getLayerSize(0);

			// calculate the temporal values for each layer
			for (int oLayer = 1; oLayer < getNumLayers(); ++oLayer) {

				const int iLayer = oLayer - 1;

				// calculate the temporals within one layer...
				#pragma omp parallel for if(useOMP)
				for (int o = 0; o < getLayerSize(oLayer); ++o) {

					// realtive offset within the input factors (needed for OMP to work)
					const int fOff = _fOff + o * getLayerSize(iLayer);

					// ... by using all inputs
					for (int i = 0; i < getLayerSize(iLayer); ++i) {

						// update output by adding inputs
						temporals[tOffO + o] += OP::get(temporals[tOffI + i], factors[fOff + i]); //temporals[tOffI + i] * factors[fOff + i];

					}

				}

				// apply post-processing
				for (int o = 0; o < getLayerSize(oLayer); ++o) {
					temporals[tOffO + o] = OP::post(temporals[tOffO + o]);
				}

				// proceed with the next temporal-value batch for input and output
				tOffI = tOffO;
				tOffO += getLayerSize(oLayer);

				// proceed with the next input-factor batch
				_fOff += getLayerSize(iLayer) * getLayerSize(oLayer);

			}

			// create output vector
			std::vector<T> vec(&temporals[tOffI], &temporals[tOffO]);
			return vec;

		}



		/** get the number of factors, based on the number and size of layers */
		int getNumFactors() const {
			int sum = 0;
			for (size_t i = 0; i < layers.size()-1; ++i) {
				sum += layers[i] * layers[i+1];
			}
			return sum;
		}

		/** get the number temporal variables needed during calculation */
		int getNumTemporals() const {
			int sum = 0;
			for (size_t i = 0; i < layers.size(); ++i) {
				sum += layers[i];
			}
			return sum;
		}

	};

}

#endif // NEURALNET_H

