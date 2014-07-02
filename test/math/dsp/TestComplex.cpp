
/*
 * TestConvolution.cpp
 *
 *  Created on: Mar 22, 2014
 *      Author: Frank Ebner
 */


#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/dsp/dft/Complex.h"
#include <complex>

namespace K {


	TEST(Complex, test) {


		std::complex<float> c1;
		Complex<float> c2;

		ASSERT_EQ(c1.real(), c2.r);
		ASSERT_EQ(c1.imag(), c2.i);

		c1.real(1);
		c1.imag(5);
		c2.r = 1;
		c2.i = 5;

		ASSERT_EQ(c1.real(), c2.r);
		ASSERT_EQ(c1.imag(), c2.i);

		c1 += c1;
		c2 += c2;

		ASSERT_EQ(c1.real(), c2.r);
		ASSERT_EQ(c1.imag(), c2.i);

		c1 -= c1;
		c2 -= c2;

		ASSERT_EQ(c1.real(), c2.r);
		ASSERT_EQ(c1.imag(), c2.i);

		c1 *= c1;
		c2 *= c2;

		ASSERT_EQ(c1.real(), c2.r);
		ASSERT_EQ(c1.imag(), c2.i);

		std::complex<float> cc1 = c1 + c1 * (c1 - c1);
		Complex<float> cc2 = c2 + c2 * (c2 - c2);

		ASSERT_EQ(cc1.real(), cc2.r);
		ASSERT_EQ(cc1.imag(), cc2.i);

		c1.real(1);
		c1.imag(-2);
		c2.real(1);
		c2.imag(-2);

		float a1 = std::sqrt( std::abs( (c1*c1).real() ) );
		float a2 = std::sqrt( std::abs( (c2*c2).real() ) );
		ASSERT_FLOAT_EQ(a1,a2);


		c1.real(-1);
		c1.imag(-2.5);
		c2.real(-1);
		c2.imag(-2.5);

		float b1 = std::sqrt( std::abs( (c1*c1).real() ) );
		float b2 = std::sqrt( std::abs( (c2*c2).real() ) );
		ASSERT_FLOAT_EQ(b1,b2);


	}

	TEST(Complex, test2) {



		std::complex<float> c11(0,5);
		std::complex<float> c12(1,2);

		Complex<float> c21(0,5);
		Complex<float> c22(1,2);

		ASSERT_EQ(c11.real(), c21.real());
		ASSERT_EQ(c11.imag(), c21.imag());
		ASSERT_EQ(c12.real(), c22.real());
		ASSERT_EQ(c12.imag(), c22.imag());

		std::complex<float> c1 =	c11*c12;
		Complex<float> c2 =			c21*c22;

		ASSERT_EQ(c1.real(), c2.real());
		ASSERT_EQ(c1.imag(), c2.imag());

	}

	TEST(Complex, test3) {


		std::complex<float> c11(0,5);
		std::complex<float> c12(1,2);
		std::complex<float> c13(2,-1);

		Complex<float> c21(0,5);
		Complex<float> c22(1,2);
		Complex<float> c23(2,-1);

		std::complex<float> c1 =	c11*c12+c11+c13-c13*c13*c11-c12;
		Complex<float> c2 =			c21*c22+c21+c23-c23*c23*c21-c22;

		ASSERT_EQ(c1.real(), c2.real());
		ASSERT_EQ(c1.imag(), c2.imag());


	}

	TEST(Complex, text4) {

		std::complex<float> c1(5,7);
		Complex<float> c2(5,7);

		c1 *= 0.5f;
		c2 *= 0.5f;

		ASSERT_EQ(c1.real(), c2.real());
		ASSERT_EQ(c1.imag(), c2.imag());

	}

}

#endif
