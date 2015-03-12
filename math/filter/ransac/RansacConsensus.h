#ifndef RANSACCONSENSUS_H
#define RANSACCONSENSUS_H

#include "RansacSampleSet.h"
#include "RansacModelParameters.h"

namespace K {

	/**
	 * calculate the consensus set, matching the model parameters
	 * estimated from the random subset
	 */
	template <typename Sample, int numParams>
	class RansacConsensus {

	public:

		virtual RansacSampleSet<Sample>* getConsensusSet(const RansacSampleSet<Sample>* allSamples, RansacModelParameters<numParams>& parameters) = 0;

	};

}

#endif // RANSACCONSENSUS_H
