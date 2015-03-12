#ifndef RANSACSAMPLESET_H
#define RANSACSAMPLESET_H

namespace K {

	/**
	 * the complete sample set
	 *  1) draw random samples from this set
	 *	2) estimate the model parameters
	 *	3) perform outlier detection
	 *	start over
	 */
	template <typename Sample>
	class RansacSampleSet {

	public:

		/** get the number of samples within the set */
		virtual uint32_t getSize() const = 0;

		/** get the idx-th sample from the set */
		virtual const Sample* getSample(const int idx) const = 0;

		/** TESTING */
		virtual void remove(const Sample* s) = 0;

	};

}

#endif // RANSACSAMPLESET_H
