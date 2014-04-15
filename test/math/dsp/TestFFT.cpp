/*
 * TestFFT.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/dsp/dft/FFT.h"
#include "../../../math/dsp/dft/FFTFixed.h"
#include "../../../math/dsp/dft/FFTRecursive.h"
#include "../../../math/dsp/dft/FFT2.h"
#include "../../../os/Time.h"


namespace K {

	void bindCurrentThreadToCore(int coreNr) {

		// whole process
		cpu_set_t my_set;
		CPU_ZERO(&my_set);
		CPU_SET(coreNr, &my_set);
		//sched_setaffinity(0, sizeof(cpu_set_t), &my_set);

		// one thread only
		//unsigned long mask = 1;
		int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &my_set);
		if (ret != 0) {std::cout << "error while settings thread affinity" << std::endl; exit(1);}

	}

	/** fill array with simple sine function */
	template <typename type> void fftFillSine(type arr[], unsigned int len) {
		for (unsigned int i = 0; i < len; ++i) {
			double t = (double) i / (double) len;
			arr[i] = sin(2 * K::PI * t) + sin(2 * K::PI * t * 4);
		}
	}

	double fftABS(Complex<double> complex) {
		return std::sqrt( std::abs( (complex*complex).real() ) );
	}


	TEST(FFT, create) {

		// create test-data
		const int SIZE = 16;
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		FFT fft(SIZE);
		Complex<double>* res = fft.getComplexFFT(sine);

//		for (unsigned int i = 0; i < SIZE; ++i) {
//			std::cout << res[i].real() << std::endl;
//			std::cout << "\t" << fftABS(res[i]) << std::endl;
//		}

		EXPECT_NEAR(0, fftABS(res[0]), 0.001);
		EXPECT_NEAR(8, fftABS(res[1]), 0.001);
		EXPECT_NEAR(0, fftABS(res[2]), 0.001);
		EXPECT_NEAR(0, fftABS(res[3]), 0.001);
		EXPECT_NEAR(8, fftABS(res[4]), 0.001);

		delete[] res;

	}

	TEST(FFT, Create2) {

		// create test-data
		const int SIZE = 16;
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		FFT2 fft(SIZE);
		ComplexBuffer<double> buf(sine, SIZE);
		fft.getComplexFFT(buf);

		ASSERT_NEAR(0, buf.abs(0), 0.001);
		ASSERT_NEAR(8, buf.abs(1), 0.001);
		ASSERT_NEAR(0, buf.abs(2), 0.001);
		ASSERT_NEAR(0, buf.abs(3), 0.001);
		ASSERT_NEAR(8, buf.abs(4), 0.001);

	}


	/** test the recursive FFT */
	TEST(FFT, recursiveFFTandIFFT) {

		const int SIZE = 512;
		FFTRecusive <SIZE> fft;

		// create test-data
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		std::complex<double>* ffted = fft.getComplexFFT(sine);
		double* iffted = fft.getIFFT(ffted);

		for (unsigned int i = 0; i < SIZE; ++i) {
			ASSERT_NEAR(iffted[i], sine[i], 0.00000001);
		}

		delete[] ffted;
		delete[] iffted;

	}

	/** test the non-recusive-fixed FFT */
	TEST(FFT, fixedFFTandIFFT) {

		const int SIZE = 512;
		FFTFixed<SIZE> fft;

		// create test-data
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		std::complex<double>* ffted = fft.getComplexFFT(sine);
		double* iffted = fft.getIFFT(ffted);

		for (unsigned int i = 0; i < SIZE; ++i) {
			ASSERT_NEAR(iffted[i], sine[i], 0.00000001);
		}

		delete[] ffted;
		delete[] iffted;

	}

	/** test the non-recursive, dynamic FFT */
	TEST(FFT, dynamicFFTandIFFT) {

		const int SIZE = 512;
		FFT fft(SIZE);

		// create test-data
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		Complex<double>* ffted = fft.getComplexFFT(sine);
		double* iffted = fft.getIFFT(ffted);

		for (unsigned int i = 0; i < SIZE; ++i) {
			ASSERT_NEAR(iffted[i], sine[i], 0.00000001);
		}

		delete[] ffted;
		delete[] iffted;

	}

	TEST(FFT, Benchmark) {

		bindCurrentThreadToCore(0);

		const unsigned int REP = 1024;
		const unsigned int SIZE = 4096;
		double sine[SIZE];
		fftFillSine(sine, SIZE);

		{

			std::cout << "recursive FFT" << std::endl;

			FFTRecusive<SIZE> fft;
			uint64_t s = K::Time::getTimeMS();
			for (unsigned int i = 0; i < REP; ++i) {
				std::complex<double>* ret = fft.getComplexFFT(sine);
				delete[] ret;
			}
			uint64_t e = K::Time::getTimeMS();
			std::cout << "exec: " << (e-s) << std::endl;

		}


		{

			std::cout << "fixed, non-recursive FFT" << std::endl;

			FFTFixed<SIZE> fft;
			uint64_t s = K::Time::getTimeMS();
			for (unsigned int i = 0; i < REP; ++i) {
				std::complex<double>* ret = fft.getComplexFFT(sine);
				delete[] ret;
			}
			uint64_t e = K::Time::getTimeMS();
			std::cout << "exec: " << (e-s) << std::endl;

		}

		{

			std::cout << "dynamic, non-recursive FFT" << std::endl;

			FFT fft(SIZE);
			uint64_t s = K::Time::getTimeMS();
			for (unsigned int i = 0; i < REP; ++i) {
				Complex<double>* ret = fft.getComplexFFT(sine);
				delete[] ret;
			}
			uint64_t e = K::Time::getTimeMS();
			std::cout << "exec: " << (e-s) << std::endl;

		}

		{
			std::cout << "complex buffer FFT" << std::endl;

			FFT2 fft(SIZE);
			ComplexBuffer<double> buf(sine, SIZE);
			uint64_t s = K::Time::getTimeMS();
			for (unsigned int i = 0; i < REP; ++i) {
				fft.getComplexFFT(buf);
				//delete[] ret;
			}
			uint64_t e = K::Time::getTimeMS();
			std::cout << "exec: " << (e-s) << std::endl;

		}


	}




}

#endif
