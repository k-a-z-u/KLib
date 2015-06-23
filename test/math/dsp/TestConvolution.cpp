/*
 * TestConvolution.cpp
 *
 *  Created on: Mar 22, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"
#include "TestDSPHelper.h"
#include "../../../math/dsp/convolution/DSPConvolution.h"
#include "../../../math/dsp/convolution/DSPConvolutionFFT.h"

#include "../../../os/Process.h"

namespace K {


	TEST(Convolution, simple) {

		Process gp("/usr/bin/gnuplot", "");
		unsigned int SIZE = 64;
		float arr1[SIZE];

		DSPConvolution conv;
		TestDSPHelper::fillSine(arr1, SIZE);

		float arr2[5] = {0.1f, 0.2f, -0.4f, 0.2f, 0.1f};

		float* arr3 = conv.convolve(arr1,SIZE, arr2,5);


		gp << "plot '-' with lines\n";
		for (unsigned int i = 0; i < SIZE+3; ++i) {
			gp << arr3[i] << "\n";
		}
		gp << "e\n";
		gp.flush();

		sleep(3);

		delete[] arr3;

	}

	TEST(Convolution, fft) {

		Process gp("/usr/bin/gnuplot", "");
		unsigned int SIZE = 64;
		float arr1[SIZE];

		DSPConvolutionFFT<float> conv;
		TestDSPHelper::fillSine(arr1, SIZE);

		float arr2[5] = {0.1f, 0.2f, -0.4f, 0.2f, 0.1f};

		conv.setImpulseResponse(arr2, 5);
		float* arr3 = conv.convolve(arr1,SIZE);


		gp << "plot '-' with lines\n";
		for (unsigned int i = 0; i < SIZE+3; ++i) {
			gp << arr3[i] << "\n";
		}
		gp << "e\n";
		gp.flush();

		sleep(3);

		delete[] arr3;

	}


}

#endif
