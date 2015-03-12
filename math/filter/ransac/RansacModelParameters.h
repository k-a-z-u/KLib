#ifndef RANSACMODELPARAMETERS_H
#define RANSACMODELPARAMETERS_H

namespace K {

	/**
	 * represents a (fixed size) set of model parameters
	 */
	template <int numParams>
	class RansacModelParameters {

	public:

		/** ctor. initialize all values with zero */
		RansacModelParameters() : values() {
			;
		}

		double& operator [] (const int idx) {return values[idx];}



	protected:

		/** the underyling values */
		double values[numParams];

	};

}

#endif // RANSACMODELPARAMETERS_H
