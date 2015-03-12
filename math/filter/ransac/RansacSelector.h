#ifndef RANSACSELECTOR_H
#define RANSACSELECTOR_H

#include "RansacSampleSet.h"

namespace K {

	/**
	 * randomly choose samples from the underlying sample-set to
	 * pass them into:
	 *		estimation
	 *		outlier-detection
	 */
	template <typename Sample>
	class RansacSelector {

	public:

		/** provide a random subset, used to estimate the model parameters */
		virtual RansacSampleSet<Sample>* getRandomSubset(const RansacSampleSet<Sample>& input) = 0;

	};

}

#endif // RANSACSELECTOR_H
