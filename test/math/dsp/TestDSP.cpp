/*
 * Windows.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/dsp/dft/DFT.h"

namespace K {

	/** fill array with simple sine function */
	void dftFillSine(double arr[], unsigned int len) {
		for (unsigned int i = 0; i < len; ++i) {
			double t = (double) i / (double) len;
			arr[i] = sin(2 * K::PI * t) + sin(2 * K::PI * t * 8);
		}
	}


	TEST(DFT, DFTandIDFT) {

		// create test-data
		double sine[128];
		dftFillSine(sine, 128);

		// output array
		DFTRectangular<double> out[128/2+1];

		DFT<double, 128> dft;
		dft.getDFT(sine, out);

		double back[128];
		dft.getIDFT(out, back);

		for (unsigned int i = 0; i < 128; ++i) {
			//std::cout << back[i] << std::endl;
		}

		for (unsigned int i = 0; i < 128; ++i) {
			ASSERT_NEAR( sine[i], back[i], 0.0000001 );
		}

	}

	TEST(DFT, removeFreq) {

		// create test-data
		double sine[128];
		dftFillSine(sine, 128);

		// output array
		DFTRectangular<double> dfted[128/2+1];

		DFT<double, 128> dft;
		dft.getDFT(sine, dfted);

		dfted[1].sin = 0;

		// back to time domain
		double idfted[128];
		dft.getIDFT(dfted, idfted);

		for (unsigned int i = 0; i < 128; ++i) {
			std::cout << idfted[i] << std::endl;
		}

	}

}

#endif
