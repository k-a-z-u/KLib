/*
 * TestIIR.cpp
 *
 *  Created on: Mar 22, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include "TestDSPHelper.h"
#include "../../../math/dsp/filter/IIRSinglePoleLowPass.h"
#include "../../../math/dsp/filter/IIRMovingAverage.h"

#include "../../../math/dsp/dft/FFT.h"
#include "../../../math/dsp/dft/DFT.h"
#include "../../../math/dsp/window/DSPWindowBlackman.h"
#include "../../../os/Process.h"

namespace K {

	int IIRget(int i) {
		return (i > 0) ? (i) : (0);
	}

	/** test the moving average filter with several sizes */
	TEST(IIR, MovingAverage) {

		// test empty moving average filter
		IIRMovingAverage<float, 1> maNone;
		for (unsigned int i = 0; i < 128; ++i) {
			EXPECT_EQ( (float) i, (float) maNone.filter( (float) i ));
		}

		// test 2 value moving average
		IIRMovingAverage<float, 2> ma2;
		for (unsigned int i = 0; i < 128; ++i) {
			EXPECT_EQ( float( IIRget(i-0)+IIRget(i-1) ) / 2.0f, (float) ma2.filter( (float) i ) );
		}

		// test 3 value moving average
		IIRMovingAverage<float, 3> ma3;
		for (unsigned int i = 0; i < 128; ++i) {
			EXPECT_EQ( float( IIRget(i-0)+IIRget(i-1)+IIRget(i-2) ) / 3.0f, (float) ma3.filter( (float) i ) );
		}

	}



	TEST(IIR, StepResponse) {

		const unsigned int size = 256;

		IIRSinglePoleLowPass<float> lp;
		IIRMovingAverage<float, 20> ma;
		DSPWindowBlackman<float, size> win;
		DFT<float, size> dft;

		float arr[size] = {0};
		arr[0] = 1.0f;
		//TestDSPHelper::fillSine(arr, size);

		float res[size] = {0};

		// test disabled filter
		for (unsigned int i = 0; i < size; ++i) {
			res[i] = lp.filter(arr[i]);
			EXPECT_EQ(arr[i], res[i]);
		}

		FFT fft(size);

		Process gp("/usr/bin/gnuplot", "");
		//gp << "set yrange[-0.1:0.1]\n";
		gp << "plot '-' with lines\n";
		float x = 0;
		lp.setCutoffFrequency(1, 20);
		for (unsigned int i = 0; i < size; ++i) {
			res[i] = ma.filter(arr[i]);
			x += res[i];
			//gp << x << "\n";
		}


		exit(0);

		//win.apply(res);
		float* spec = fft.getRealFFT(res);
		//DFTRectangular<float> spec[size];
		//dft.getDFT(res, spec);

		for (unsigned int i = 0; i < size/2+1; ++i) {
			gp << spec[i] << "\n";
		}
		gp << "e\n";
		gp.flush();;
		delete[] spec;

		sleep(5);

	}

}

#endif
