#ifndef COMPLEXBUFFER_H
#define COMPLEXBUFFER_H

namespace K {

	/**
	 * @brief this is a helper class for the FFT that represents
	 * complex values by using two arays. one for the real and one
	 * for the imaginary parts.
	 *
	 * as the user will mainly have only real values to deal with,
	 * using two arrays he may direcly provide his real data and
	 * no copying into std::complex or similar is needed.
	 */
	template <typename type> class ComplexBuffer {

	public:

		ComplexBuffer(type* real, unsigned int size) {

			realValues = real;
			freeReal = false;

			imagValues = new type[size];
			memset(imagValues, 0, sizeof(type)*size);
			freeImag = true;

		}

		/** swap the two given indices, both real and imaginary parts */
		void swap(unsigned int i, unsigned int j) {

			type tmp;

			tmp = realValues[i];
			realValues[i] = realValues[j];
			realValues[j] = tmp;

			tmp = imagValues[i];
			imagValues[i] = imagValues[j];
			imagValues[j] = tmp;

		}

		/** get the real value for the given index */
		type real(unsigned int idx) const {return realValues[idx];}

		/** get the complex value for the given index */
		type imag(unsigned int idx) const {return imagValues[idx];}


		/** get the given index as std::complex */
		std::complex<type> asStdComplex(unsigned int idx) {
			return std::complex<type>(real(idx), imag(idx));
		}

		/** get the absolute (real only) value for the given index */
		type abs(unsigned int idx) {
			type a = real(idx);
			type b = imag(idx);
			type c = a*a - b*b;
			return std::sqrt( std::abs( c ) );
		}

		/** dtor */
		~ComplexBuffer() {
			if (freeImag) {delete[] imagValues;}
			if (freeReal) {delete[] realValues;}
			imagValues = 0;
			realValues = 0;
		}

	private:

		friend class FFT2;

		/** buffer for the real-values */
		type* realValues;
		bool freeReal;

		/** buffer for the imaginary values */
		type* imagValues;
		bool freeImag;

	};

}

#endif // COMPLEXBUFFER_H
