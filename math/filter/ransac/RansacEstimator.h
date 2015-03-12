#ifndef RANSACESTIMATOR_H
#define RANSACESTIMATOR_H

#include "RansacSampleSet.h"
#include "RansacModelParameters.h"

namespace K {

	/**
	 * estimate the model parameters based on a random subset
	 */
	template <typename Sample, int numParams>
	class RansacEstimator {

	public:

		/** estimatethe model parameters based on the given random subset */
		virtual RansacModelParameters<numParams>* getEstimatedModelParameters(const RansacSampleSet<Sample>& subset) = 0;

	};

}

#endif // RANSACESTIMATOR_H
