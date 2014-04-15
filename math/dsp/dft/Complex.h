#ifndef K_MATH_DSP_DFT_COMPLEX_H
#define K_MATH_DSP_DFT_COMPLEX_H

namespace K {

	template <typename type> struct Complex {

		/** empty ctor */
		Complex() : r(0), i(0) {;}

		/** ctor */
		Complex(type real, type imag) : r(real), i(imag) {
			;
		}

		/** copy ctor */
		Complex(const Complex& other) : r(other.r), i(other.i) {
			;
		}

		void operator += ( const Complex<type> other ) {
			this->r += other.r;
			this->i += other.i;
		}

		void operator -= ( const Complex<type> other ) {
			this->r -= other.r;
			this->i -= other.i;
		}

		void operator *= ( const Complex<type> other ) {
			type _r = r*other.r - i*other.i;
			type _i = r*other.i + i*other.r;
			this->r = _r;
			this->i = _i;
		}

		const Complex<type> operator * ( const Complex<type> other ) const {
			Complex<type> cpy(*this);
			cpy *= other;
			return cpy;
		}

		const Complex<type> operator + ( const Complex<type> other ) const {
			Complex<type> cpy(*this);
			cpy += other;
			return cpy;
		}

		const Complex<type> operator - ( const Complex<type> other ) const {
			Complex<type> cpy(*this);
			cpy -= other;
			return cpy;
		}

		void real(type r) {this->r = r;}
		void imag(type i) {this->i = i;}

		type real() const {return r;}
		type imag() const {return i;}


		/** the real part */
		type r;

		/** the imaginary part */
		type i;


	};

}

#endif // K_MATH_DSP_DFT_COMPLEX_H
