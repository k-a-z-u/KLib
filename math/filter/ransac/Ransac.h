#ifndef RANSAC_H
#define RANSAC_H

#include "RansacSampleSet.h"
#include "RansacModelParameters.h"
#include "RansacSelector.h"
#include "RansacConsensus.h"
#include "RansacEstimator.h"

namespace K {

	template <typename Sample, int numParams>
	class Ransac {

	public:

		RansacSampleSet<Sample>* getConsensus(
				 RansacSampleSet<Sample>& input,
				RansacSelector<Sample>* selector,
				RansacEstimator<Sample, numParams>* estimator,
				RansacConsensus<Sample, numParams>* consensus) {

			RansacSampleSet<Sample>* bestConsensus = nullptr;

			for (int i = 0; i < 100; ++i) {

				// draw a random subset
				RansacSampleSet<Sample>* subset = selector->getRandomSubset(input);

				// estimate model parameters from this subset
				RansacModelParameters<numParams>* params = estimator->getEstimatedModelParameters(*subset);

				// calculate the consensus set
				RansacSampleSet<Sample>* consensusSet = consensus->getConsensusSet(&input, *params);
				std::cout << "consensus: " << consensusSet->getSize() << std::endl;

				// cleanups?
				if (consensusSet->getSize() < input.getSize() / 10) {
					for (int i = 0; i < subset->getSize(); ++i) {
						const Sample* s = subset->getSample(i);
						input.remove(s);
					}
				}

				// better?
				if (!bestConsensus || consensusSet->getSize() > bestConsensus->getSize()) {
					bestConsensus = consensusSet;
					std::cout << "params:" << std::endl;
				} else {
					delete consensusSet;
				}



				for (int i = 0; i < numParams; ++i) {
					std::cout << "\t" << (*params)[i] << std::endl;
				}

				std::cout << "best: " << bestConsensus->getSize() << std::endl;

				// cleanups
				delete params;
				delete subset;

			}

			return bestConsensus;

		}

	};

}

#endif // RANSAC_H
